#include "pch.h"
#include "DX11BulletPhisics.h"
#include "CP_RigidBody.h"
#include "CollisionInfo.h"
#include "GameObject.h"

using namespace HashiTaku;

constexpr int  DEFAULT_MAX_SUBSTEPS(10);	// �f�t�H���g�ő�T�u�X�e�b�v��

bool DX11BulletPhisics::GetDisplay() const
{
#ifdef EDIT
	return pDebugDraw->GetIsDisplay();
#endif // EDIT

	return false;
}

DirectX::SimpleMath::Vector3 DX11BulletPhisics::GetGravityValue() const
{
	return gravityValue;
}

DX11BulletPhisics::DX11BulletPhisics() : maxSubSteps(DEFAULT_MAX_SUBSTEPS), gravityValue(0.0f, Mathf::gravity, 0.0f)
{
}

bool DX11BulletPhisics::IsExistCollObjInWorld(btCollisionObject& _checkCollObj)
{
	u_int numCollisionObjects = pDynamicsWorld->getNumCollisionObjects();

	for (u_int i = 0; i < numCollisionObjects; i++)
	{
		btCollisionObject* obj = pDynamicsWorld->getCollisionObjectArray()[i];

		if (obj == &_checkCollObj)	// ��������
		{
			return true;
		}
	}

	return false;
}

void DX11BulletPhisics::UpdateColExit()
{
	// Exit�̏���������

	// ���̂���
	for (auto& pRb : sceneRigidbodys)
	{
		// �ՓˏI������Rb�R���|�[�l���g���擾
		std::vector<const CP_RigidBody*> exitRbCps;
		CollisionTypeJudge& colJudge = pRb->GetColTypeJudge();
		colJudge.GetExitObjectList(exitRbCps);

		// �I������Rb�R���|�[�l���g��OnCollisionExit�ɓn��
		for (auto& pExitRb : exitRbCps)
		{
			CollisionInfo otherColInfo;
			otherColInfo.pRigidBodyCp = pExitRb;
			pRb->GetGameObject().OnCollisionExit(otherColInfo);
		}

		// ���t���[���ׂ̈ɍX�V���Ă���
		colJudge.SetupForNextFrame();
	}

	// Ghost
	for (auto& pGhost : sceneGhostObjects)
	{
		// �ՓˏI������Rb�R���|�[�l���g���擾
		std::vector<const CP_RigidBody*> exitRbCps;
		CollisionTypeJudge& colJudge = pGhost->GetColTypeJudge();
		colJudge.GetExitObjectList(exitRbCps);

		// �I������Rb�R���|�[�l���g��OnCollisionExit�ɓn��
		for (auto& pExitRb : exitRbCps)
		{
			CollisionInfo otherColInfo;
			otherColInfo.pRigidBodyCp = pExitRb;
			pGhost->GetGameObject().OnCollisionExit(otherColInfo);
		}

		// ���t���[���ׂ̈ɍX�V���Ă���
		colJudge.SetupForNextFrame();
	}
}

void DX11BulletPhisics::Init()
{
	// �C���X�^���X�쐬
	pCollisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
	pDispatcher = std::make_unique<btCollisionDispatcher>(pCollisionConfiguration.get());
	pBroadphase = std::make_unique<btDbvtBroadphase>();
	pSolver = std::make_unique<btSequentialImpulseConstraintSolver>();
	pDynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
		pDispatcher.get(),
		pBroadphase.get(),
		pSolver.get(),
		pCollisionConfiguration.get()
	);
	pGhostPairCallback = std::make_unique<btGhostPairCallback>();
	
	pContactCallBack = std::make_unique<BulletContactCallBack>();

	// �d�͐ݒ�
	pDynamicsWorld->setGravity(Bullet::ToBtVector3(gravityValue));


	// Ghost���b�v���m���Z�b�g
	pDynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(pGhostPairCallback.get());


#ifdef EDIT
	// �f�o�b�O�`��N���X�Z�b�g
	pDebugDraw = std::make_unique<BulletDebugDraw>();
	pDynamicsWorld->setDebugDrawer(pDebugDraw.get());
#endif // EDIT

}

void DX11BulletPhisics::Update()
{
	// �V�~�����[�V�����̃X�e�b�v��i�߂�
	pDynamicsWorld->stepSimulation(
		Bullet::ToBtScalar(MainApplication::DeltaTime()),
		maxSubSteps
	);

	// �V�~�����[�V�����Ői�߂�Bullet�̃g�����X�t�H�[����Dx�������킹��
	UpdateTransformBtToDx();

	// �R���W�����̏Փ˔����ʒm����
	CollisionCallBack();
}

void DX11BulletPhisics::CollisionCallBack()
{
	// ���[���h��ԓ��̔��������
	const btCollisionObjectArray& collisionObjects = pDynamicsWorld->getCollisionObjectArray();
	u_int collisionCnt = pDynamicsWorld->getNumCollisionObjects();
	for (u_int c_i = 0; c_i < collisionCnt; c_i++)
	{
		btCollisionObject& collObj = *collisionObjects[c_i];
		pDynamicsWorld->contactTest(&collObj, *pContactCallBack);
	}

	// �ՓˏI���̏���
	UpdateColExit();
}

void DX11BulletPhisics::Draw()
{
#ifdef EDIT
	if (!pDebugDraw->GetIsDisplay()) return;

	//�f�o�b�O�`����Ăяo��
	pDynamicsWorld->debugDrawWorld();
#endif
}

void DX11BulletPhisics::AddCollObj(CP_RigidBody& _rigidBodyCp, int _groupId)
{
	// ���[���h��ԓ��ɂȂ������珈�����Ȃ�
	btCollisionObject* pColObj = &_rigidBodyCp.GetCollisionObject();
	if (IsExistCollObjInWorld(*pColObj))
	{
		HASHI_DEBUG_LOG("���[���h��ԓ��ɃR���W���������ɂ���܂�");
		return;
	}

	btRigidBody* rb = btRigidBody::upcast(pColObj);
	if (rb)	// ���̂Ȃ�
	{
		// ���[���h�ɒǉ�
		pDynamicsWorld->addRigidBody(rb);
		// ���̃��X�g�ɒǉ�
		sceneRigidbodys.push_back(&_rigidBodyCp);
	}
	else
	{
		// ���[���h�ɒǉ�
		pDynamicsWorld->addCollisionObject(pColObj);
		// GhostObject���X�g�ɒǉ�
		sceneGhostObjects.push_back(&_rigidBodyCp);
	}

	HASHI_DEBUG_LOG("�R���C�_�[�Z�b�g�@����" + std::to_string(pDynamicsWorld->getNumCollisionObjects()));
}

void DX11BulletPhisics::RemoveCollObj(CP_RigidBody& _rigidBodyCp)
{
	// ���[���h��ԓ��ɂȂ������珈�����Ȃ�
	btCollisionObject* pColObj = &_rigidBodyCp.GetCollisionObject();
	if (!IsExistCollObjInWorld(*pColObj))
	{
		HASHI_DEBUG_LOG("���[���h��ԓ��Ɏw�肵���R���W����������܂���");
		return;
	}

	btRigidBody* rb = btRigidBody::upcast(pColObj);

	if (rb)	// ���̂Ȃ�
	{
		// ���[���h����폜����
		pDynamicsWorld->removeRigidBody(rb);
		sceneRigidbodys.remove(&_rigidBodyCp);
	}
	else
	{
		pDynamicsWorld->removeCollisionObject(pColObj);
		sceneGhostObjects.remove(&_rigidBodyCp);
	}

	HASHI_DEBUG_LOG("�R���C�_�[�폜�@����" + std::to_string(pDynamicsWorld->getNumCollisionObjects()));
}

void DX11BulletPhisics::UpdateTransformBtToDx()
{
	// ���̂̂ݍX�V����
	for (auto& pRbCp : sceneRigidbodys)
		pRbCp->SetToDXTransform();
}

void DX11BulletPhisics::UpdateTransformDxToBt()
{
	// �Փ˃I�u�W�F�N�g�S�čX�V����

	for (auto& pRbCp : sceneRigidbodys)
		pRbCp->SetToBtTransform();

	for (auto& pRbCp : sceneGhostObjects)
		pRbCp->SetToBtTransform();
}

void DX11BulletPhisics::SetDisplay(bool _setBool)
{
#ifdef EDIT
	pDebugDraw->SetIsDisplay(_setBool);
#endif // EDIT
}

u_int DX11BulletPhisics::GetCollObjCnt() const
{
	return pDynamicsWorld->getNumCollisionObjects();
}

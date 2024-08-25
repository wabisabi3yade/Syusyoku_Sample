#include "pch.h"
#include "DX11BulletPhisics.h"

#include "MainApplication.h"

using namespace HashiTaku;

constexpr u_int DEFAULT_MAX_SUBSTEPS(10);	// �f�t�H���g�ő�T�u�X�e�b�v��

DX11BulletPhisics::DX11BulletPhisics() : maxSubSteps(DEFAULT_MAX_SUBSTEPS)
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

void DX11BulletPhisics::Init()
{
	// �C���X�^���X�쐬
	pBroadphase = std::make_unique<btDbvtBroadphase>();
	pCollisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
	pDispatcher = std::make_unique<btCollisionDispatcher>(pCollisionConfiguration.get());
	pSolver = std::make_unique<btSequentialImpulseConstraintSolver>();
	pDynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
		pDispatcher.get(),
		pBroadphase.get(),
		pSolver.get(),
		pCollisionConfiguration.get()
	);
	pDebugDraw = std::make_unique<BulletDebugDraw>();

	// �d�͐ݒ�
	pDynamicsWorld->setGravity(Bullet::ToBtVector3({ 0.0f, Mathf::gravity, 0.0f }));

	// �f�o�b�O�`��N���X�Z�b�g
	pDynamicsWorld->setDebugDrawer(pDebugDraw.get());
}

void DX11BulletPhisics::Update()
{
	// �V�~�����[�V�����̃X�e�b�v��i�߂�
	pDynamicsWorld->stepSimulation(
		Bullet::ToBtScalar(MainApplication::DeltaTime()),
		maxSubSteps
	);
}

void DX11BulletPhisics::Draw()
{
	// �f�o�b�O�`����Ăяo��
	pDynamicsWorld->debugDrawWorld();

	// �f�o�b�O�`�������
	pDebugDraw->Draw();
}

void DX11BulletPhisics::AddCollObj(btCollisionObject& _collObj)
{
	// ���[���h��ԓ��ɂȂ������珈�����Ȃ�
	if (IsExistCollObjInWorld(_collObj))
	{
		HASHI_DEBUG_LOG("���[���h��ԓ��ɃR���W���������ɂ���܂�");
		return;
	}

	btRigidBody* rb = dynamic_cast<btRigidBody*>(&_collObj);

	if (rb)	// ���̂Ȃ�
		pDynamicsWorld->addRigidBody(rb);

	else
		pDynamicsWorld->addCollisionObject(&_collObj);
}

void DX11BulletPhisics::RemoveCollObj(btCollisionObject& _collObj)
{
	// ���[���h��ԓ��ɂȂ������珈�����Ȃ�
	if (!IsExistCollObjInWorld(_collObj))
	{
		HASHI_DEBUG_LOG("���[���h��ԓ��ɃR���W����������܂���");
		return;
	}

	btRigidBody* rb = dynamic_cast<btRigidBody*>(&_collObj);

	if (rb)	// ���̂Ȃ�
		pDynamicsWorld->removeRigidBody(rb);

	else
		pDynamicsWorld->removeCollisionObject(&_collObj);
}

u_int DX11BulletPhisics::GetCollObjCnt() const
{
	return pDynamicsWorld->getNumCollisionObjects();
}

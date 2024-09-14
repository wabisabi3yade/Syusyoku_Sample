#include "pch.h"
#include "DX11BulletPhisics.h"


using namespace HashiTaku;

constexpr u_int DEFAULT_MAX_SUBSTEPS(10);	// �f�t�H���g�ő�T�u�X�e�b�v��

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
	pDynamicsWorld->setGravity(Bullet::ToBtVector3(gravityValue));

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

	// �Փ˃I�u�W�F�N�g�̐����擾
	int numManifolds = pDynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold = pDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* objA = contactManifold->getBody0();
		const btCollisionObject* objB = contactManifold->getBody1();

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);

			if (pt.getDistance() < 0.0f) {
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;

				std::cout << "Collision detected between objects!" << std::endl;
				std::cout << "Contact point A: " << ptA.getX() << ", " << ptA.getY() << ", " << ptA.getZ() << std::endl;
				std::cout << "Contact point B: " << ptB.getX() << ", " << ptB.getY() << ", " << ptB.getZ() << std::endl;
				std::cout << "Collision normal: " << normalOnB.getX() << ", " << normalOnB.getY() << ", " << normalOnB.getZ() << std::endl;
			}
		}
	}
}

void DX11BulletPhisics::Draw()
{
	// �f�o�b�O�`����Ăяo��
	pDynamicsWorld->debugDrawWorld();
}

void DX11BulletPhisics::AddCollObj(btCollisionObject& _collObj, int _groupId)
{
	// ���[���h��ԓ��ɂȂ������珈�����Ȃ�
	if (IsExistCollObjInWorld(_collObj))
	{
		HASHI_DEBUG_LOG("���[���h��ԓ��ɃR���W���������ɂ���܂�");
		return;
	}

	btRigidBody* rb = dynamic_cast<btRigidBody*>(&_collObj);

	if (rb)	// ���̂Ȃ�
		pDynamicsWorld->addRigidBody(rb, 0x01, 0x01);

	else
		pDynamicsWorld->addCollisionObject(&_collObj, 0x01, 0x01);
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

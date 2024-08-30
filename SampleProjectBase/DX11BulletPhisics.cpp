#include "pch.h"
#include "DX11BulletPhisics.h"

#include "MainApplication.h"

using namespace HashiTaku;

constexpr u_int DEFAULT_MAX_SUBSTEPS(10);	// デフォルト最大サブステップ数

DX11BulletPhisics::DX11BulletPhisics() : maxSubSteps(DEFAULT_MAX_SUBSTEPS)
{
}

bool DX11BulletPhisics::IsExistCollObjInWorld(btCollisionObject& _checkCollObj)
{
	u_int numCollisionObjects = pDynamicsWorld->getNumCollisionObjects();

	for (u_int i = 0; i < numCollisionObjects; i++) 
	{
		btCollisionObject* obj = pDynamicsWorld->getCollisionObjectArray()[i];

		if (obj == &_checkCollObj)	// あったら
		{
			return true; 
		}
	}

	return false;
}

void DX11BulletPhisics::Init()
{
	// インスタンス作成
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

	// 重力設定
	pDynamicsWorld->setGravity(Bullet::ToBtVector3({ 0.0f, Mathf::gravity, 0.0f }));

	// デバッグ描画クラスセット
	pDynamicsWorld->setDebugDrawer(pDebugDraw.get());
}

void DX11BulletPhisics::Update()
{
	// シミュレーションのステップを進める
	pDynamicsWorld->stepSimulation(
		Bullet::ToBtScalar(MainApplication::DeltaTime()),
		maxSubSteps
	);
}

void DX11BulletPhisics::Draw()
{
	// デバッグ描画を呼び出す
	pDynamicsWorld->debugDrawWorld();

	// デバッグ描画を処理
	pDebugDraw->Draw();
}

void DX11BulletPhisics::AddCollObj(btCollisionObject& _collObj)
{
	// ワールド空間内になかったら処理しない
	if (IsExistCollObjInWorld(_collObj))
	{
		HASHI_DEBUG_LOG("ワールド空間内にコリジョンが既にあります");
		return;
	}

	btRigidBody* rb = dynamic_cast<btRigidBody*>(&_collObj);

	if (rb)	// 剛体なら
		pDynamicsWorld->addRigidBody(rb);

	else
		pDynamicsWorld->addCollisionObject(&_collObj);
}

void DX11BulletPhisics::RemoveCollObj(btCollisionObject& _collObj)
{
	// ワールド空間内になかったら処理しない
	if (!IsExistCollObjInWorld(_collObj))
	{
		HASHI_DEBUG_LOG("ワールド空間内にコリジョンがありません");
		return;
	}

	btRigidBody* rb = dynamic_cast<btRigidBody*>(&_collObj);

	if (rb)	// 剛体なら
		pDynamicsWorld->removeRigidBody(rb);

	else
		pDynamicsWorld->removeCollisionObject(&_collObj);
}

u_int DX11BulletPhisics::GetCollObjCnt() const
{
	return pDynamicsWorld->getNumCollisionObjects();
}

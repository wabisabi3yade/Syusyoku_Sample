#include "pch.h"
#include "DX11BulletPhisics.h"
#include "CP_RigidBody.h"
#include "CollisionInfo.h"
#include "GameObject.h"

using namespace HashiTaku;

constexpr int  DEFAULT_MAX_SUBSTEPS(10);	// デフォルト最大サブステップ数

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

		if (obj == &_checkCollObj)	// あったら
		{
			return true;
		}
	}

	return false;
}

void DX11BulletPhisics::UpdateColExit()
{
	// Exitの処理をする

	// 剛体から
	for (auto& pRb : sceneRigidbodys)
	{
		// 衝突終了したRbコンポーネントを取得
		std::vector<const CP_RigidBody*> exitRbCps;
		CollisionTypeJudge& colJudge = pRb->GetColTypeJudge();
		colJudge.GetExitObjectList(exitRbCps);

		// 終了したRbコンポーネントをOnCollisionExitに渡す
		for (auto& pExitRb : exitRbCps)
		{
			CollisionInfo otherColInfo;
			otherColInfo.pRigidBodyCp = pExitRb;
			pRb->GetGameObject().OnCollisionExit(otherColInfo);
		}

		// 次フレームの為に更新しておく
		colJudge.SetupForNextFrame();
	}

	// Ghost
	for (auto& pGhost : sceneGhostObjects)
	{
		// 衝突終了したRbコンポーネントを取得
		std::vector<const CP_RigidBody*> exitRbCps;
		CollisionTypeJudge& colJudge = pGhost->GetColTypeJudge();
		colJudge.GetExitObjectList(exitRbCps);

		// 終了したRbコンポーネントをOnCollisionExitに渡す
		for (auto& pExitRb : exitRbCps)
		{
			CollisionInfo otherColInfo;
			otherColInfo.pRigidBodyCp = pExitRb;
			pGhost->GetGameObject().OnCollisionExit(otherColInfo);
		}

		// 次フレームの為に更新しておく
		colJudge.SetupForNextFrame();
	}
}

void DX11BulletPhisics::Init()
{
	// インスタンス作成
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

	// 重力設定
	pDynamicsWorld->setGravity(Bullet::ToBtVector3(gravityValue));


	// Ghostラップ検知をセット
	pDynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(pGhostPairCallback.get());


#ifdef EDIT
	// デバッグ描画クラスセット
	pDebugDraw = std::make_unique<BulletDebugDraw>();
	pDynamicsWorld->setDebugDrawer(pDebugDraw.get());
#endif // EDIT

}

void DX11BulletPhisics::Update()
{
	// シミュレーションのステップを進める
	pDynamicsWorld->stepSimulation(
		Bullet::ToBtScalar(MainApplication::DeltaTime()),
		maxSubSteps
	);

	// シミュレーションで進めたBulletのトランスフォームにDx側を合わせる
	UpdateTransformBtToDx();

	// コリジョンの衝突判定を通知する
	CollisionCallBack();
}

void DX11BulletPhisics::CollisionCallBack()
{
	// ワールド空間内の判定を処理
	const btCollisionObjectArray& collisionObjects = pDynamicsWorld->getCollisionObjectArray();
	u_int collisionCnt = pDynamicsWorld->getNumCollisionObjects();
	for (u_int c_i = 0; c_i < collisionCnt; c_i++)
	{
		btCollisionObject& collObj = *collisionObjects[c_i];
		pDynamicsWorld->contactTest(&collObj, *pContactCallBack);
	}

	// 衝突終了の処理
	UpdateColExit();
}

void DX11BulletPhisics::Draw()
{
#ifdef EDIT
	if (!pDebugDraw->GetIsDisplay()) return;

	//デバッグ描画を呼び出す
	pDynamicsWorld->debugDrawWorld();
#endif
}

void DX11BulletPhisics::AddCollObj(CP_RigidBody& _rigidBodyCp, int _groupId)
{
	// ワールド空間内になかったら処理しない
	btCollisionObject* pColObj = &_rigidBodyCp.GetCollisionObject();
	if (IsExistCollObjInWorld(*pColObj))
	{
		HASHI_DEBUG_LOG("ワールド空間内にコリジョンが既にあります");
		return;
	}

	btRigidBody* rb = btRigidBody::upcast(pColObj);
	if (rb)	// 剛体なら
	{
		// ワールドに追加
		pDynamicsWorld->addRigidBody(rb);
		// 剛体リストに追加
		sceneRigidbodys.push_back(&_rigidBodyCp);
	}
	else
	{
		// ワールドに追加
		pDynamicsWorld->addCollisionObject(pColObj);
		// GhostObjectリストに追加
		sceneGhostObjects.push_back(&_rigidBodyCp);
	}

	HASHI_DEBUG_LOG("コライダーセット　現在" + std::to_string(pDynamicsWorld->getNumCollisionObjects()));
}

void DX11BulletPhisics::RemoveCollObj(CP_RigidBody& _rigidBodyCp)
{
	// ワールド空間内になかったら処理しない
	btCollisionObject* pColObj = &_rigidBodyCp.GetCollisionObject();
	if (!IsExistCollObjInWorld(*pColObj))
	{
		HASHI_DEBUG_LOG("ワールド空間内に指定したコリジョンがありません");
		return;
	}

	btRigidBody* rb = btRigidBody::upcast(pColObj);

	if (rb)	// 剛体なら
	{
		// ワールドから削除する
		pDynamicsWorld->removeRigidBody(rb);
		sceneRigidbodys.remove(&_rigidBodyCp);
	}
	else
	{
		pDynamicsWorld->removeCollisionObject(pColObj);
		sceneGhostObjects.remove(&_rigidBodyCp);
	}

	HASHI_DEBUG_LOG("コライダー削除　現在" + std::to_string(pDynamicsWorld->getNumCollisionObjects()));
}

void DX11BulletPhisics::UpdateTransformBtToDx()
{
	// 剛体のみ更新する
	for (auto& pRbCp : sceneRigidbodys)
		pRbCp->SetToDXTransform();
}

void DX11BulletPhisics::UpdateTransformDxToBt()
{
	// 衝突オブジェクト全て更新する

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

#include "pch.h"
#include "BulletContactCallBack.h"
#include "GameObject.h"
#include "CollisionInfo.h"

btScalar BulletContactCallBack::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
{
	using namespace HashiTaku::Bullet;

	// 衝突オブジェクトを取得
	const btCollisionObject& objA = *colObj0Wrap->getCollisionObject();
	const btCollisionObject& objB = *colObj1Wrap->getCollisionObject();

	// 衝突したどちらかだけが以下の処理を行えばいいのでアドレスが大きかったら行うように制限する
	if (&objA < &objB) return 0;

	// 衝突情報を取得
	CollisionInfo infoA;
	infoA.pRigidBodyCp = static_cast<CP_RigidBody*>(objA.getUserPointer());
	infoA.contactPoint = ToDXVector3(cp.getPositionWorldOnA());
	CollisionInfo infoB;
	infoB.pRigidBodyCp = static_cast<CP_RigidBody*>(objB.getUserPointer());
	infoB.contactPoint = ToDXVector3(cp.getPositionWorldOnB());

	// 衝突タイプを判定してそれぞれの処理を行わせる
	GameObject& gameObjA = infoA.pRigidBodyCp->GetGameObject();
	GameObject& gameObjB = infoB.pRigidBodyCp->GetGameObject();
	CollisionTypeJudge::ColType colType = infoA.pRigidBodyCp->GetColTypeJudge().JudgeColKind(*infoB.pRigidBodyCp);

	// 衝突タイプによる処理
	switch (colType)
	{
	case CollisionTypeJudge::ColType::Already: // このフレームで2回目の処理なら 
		break;

	case CollisionTypeJudge::ColType::Enter: // 衝突開始時なら
		gameObjA.OnCollisionEnter(infoB);
		gameObjB.OnCollisionEnter(infoA);
		break;

	case CollisionTypeJudge::ColType::Stay: // 衝突中なら
		gameObjA.OnCollisionStay(infoB);
		gameObjB.OnCollisionStay(infoA);
		break;

	default:
		assert(!"範囲外の値が入力されています");
		break;
	}

	return 0;
}

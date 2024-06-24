#include "pch.h"
#include "CollisionChecker.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

bool CollisionChecker::SphereCollision(Collider& _sphere, Collider& _other)
{
	bool isHit = false;
	
	switch (_other.GetType())	// もう一つの当たり判定の種類を取得する
	{
	case Collider::Type::Box:
		// ボックスとの判定
		break;

	case Collider::Type::Sphere:
		isHit = SphereCollider::CollisionSphere(_sphere, _other);	// 球同士の判定
		break;
	}

	return isHit;
}
CollisionChecker::~CollisionChecker()
{
	colliders.clear();
}

void CollisionChecker::CollisionCheck()
{
	// 当たり判定を取るものと取らない物で分ける
	std::vector<Collider*> checkColliders;	// とる配列
	for (auto col : colliders)
	{
		col->ResetColliders();

		// 条件式
		if (col->GetGameObject().GetIsActive() == false) continue;	// アクティブ状態でないなら

		checkColliders.push_back(col);	// チェック配列に追加
	}

	// 一つずつ見ていく
	for (u_int i = 0; i < checkColliders.size() - 1; i++)
	{
		// 一個目の当たり判定取得
		Collider& col1 = *checkColliders[i];
		
		// タイプで当たり判定の関数を決める
		bool (*pFunc)(Collider&, Collider&);	// 関数ポインタ
		switch (checkColliders[i]->GetType())
		{
		case Collider::Type::Box:	break;

		case Collider::Type::Sphere: pFunc = &SphereCollision; break;

		default: break;
		}

		// 1つずつ決めていく
		for (u_int j = i + 1; j < checkColliders.size(); j++)
		{
			Collider& col2 = *checkColliders[j];

			pFunc(col1, col2);	// 当たり判定を取る
		}
	}
}

void CollisionChecker::AddCollider(Collider& _collider)
{
	// 同じやつがあるか確認する
	auto itr = std::find(colliders.begin(), colliders.end(), &_collider);
	if (itr != colliders.end())
	{
		ImGuiDebugLog::Add("同じコライダーは追加できません");
		return;
	}

	colliders.push_back(&_collider);

}

void CollisionChecker::PopCollider(Collider& _collider)
{
	// 同じやつがあるか確認する
	auto itr = std::find(colliders.begin(), colliders.end(), &_collider);
	if (itr == colliders.end())
	{
		ImGuiDebugLog::Add("削除するコライダーがありません");
		return;
	}

	colliders.erase(itr);	// 削除する
}


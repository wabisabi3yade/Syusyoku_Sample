#include "pch.h"
#include "CollisionChecker.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

bool CollisionChecker::SphereCollision(CP_Collider& _sphere, CP_Collider& _other)
{
	bool isHit = false;
	
	switch (_other.GetType())	// もう一つの当たり判定の種類を取得する
	{
	case CP_Collider::Type::Box:
		CP_SphereCollider::CollisionBox(_sphere, _other);	// 球とボックスの判定
		break;

	case CP_Collider::Type::Sphere:
		isHit = CP_SphereCollider::CollisionSphere(_sphere, _other);	// 球同士の判定
		break;
	}

	return isHit;
}
bool CollisionChecker::BoxCollision(CP_Collider& _box, CP_Collider& _other)
{
	bool isHit = false;

	switch (_other.GetType())	// もう一つの当たり判定の種類を取得する
	{
	case CP_Collider::Type::Box:
		isHit = CP_BoxCollider::CollisionBox(_box, _other);	// ボックス同士の判定
		break;

	case CP_Collider::Type::Sphere:
		isHit = CP_SphereCollider::CollisionBox(_other, _box);	// 球とボックスの判定
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
	if (colliders.size() == 0) return;

	// 当たり判定を取るものと取らない物で分ける
	std::vector<CP_Collider*> checkColliders;	// とる配列
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
		CP_Collider& col1 = *checkColliders[i];
		
		// タイプで当たり判定の関数を決める
		bool (*pFunc)(CP_Collider&, CP_Collider&);	// 関数ポインタ
		switch (checkColliders[i]->GetType())
		{
		// ボックス
		case CP_Collider::Type::Box: pFunc = &BoxCollision; break;

		// 球
		case CP_Collider::Type::Sphere: pFunc = &SphereCollision; break;

		default: break;
		}

		// 1つずつ決めていく
		for (u_int j = i + 1; j < checkColliders.size(); j++)
		{
			CP_Collider& col2 = *checkColliders[j];

			assert(pFunc == nullptr);

			pFunc(col1, col2);	// 当たり判定を取る
		}
	}
}

void CollisionChecker::AddCollider(CP_Collider& _collider)
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

void CollisionChecker::PopCollider(CP_Collider& _collider)
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


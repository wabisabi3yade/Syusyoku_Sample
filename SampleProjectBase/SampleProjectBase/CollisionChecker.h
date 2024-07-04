#pragma once
#include "CP_Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

// シーン内のあたり判定同士の判定を取るクラス
class CollisionChecker
{
	std::vector<CP_Collider*> colliders;	// シーン内の全てのあたり判定
	
	// 関数ポインタを使用するためにstatic関数
	// 1つ目が球だった時のコリジョン
	static bool SphereCollision(CP_Collider& _sphere, CP_Collider& _other);
	
	static bool BoxCollision(CP_Collider& _box, CP_Collider& _other);
public:
	CollisionChecker(){};
	~CollisionChecker();

	void CollisionCheck();	// 毎フレームあたり判定同士の確認する
	
	void AddCollider(CP_Collider& _collider);	// コライダーを追加
	void PopCollider(CP_Collider& _collider);	// コライダーを削除する	
};


#pragma once
#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

// シーン内のあたり判定同士の判定を取るクラス
class CollisionChecker
{
	std::vector<Collider*> colliders;	// シーン内の全てのあたり判定
	
	// 関数ポインタを使用するためにstatic関数
	// 1つ目が球だった時のコリジョン
	static bool SphereCollision(Collider& _sphere, Collider& _other);
	
public:
	CollisionChecker(){};
	~CollisionChecker();

	void CollisionCheck();	// 毎フレームあたり判定同士の確認する
	
	void AddCollider(Collider& _collider);	// コライダーを追加
	void PopCollider(Collider& _collider);	// コライダーを削除する	
};


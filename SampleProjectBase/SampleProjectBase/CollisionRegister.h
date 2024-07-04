#pragma once
#include "Singleton_Base.h"

class CP_Collider;
class CollisionChecker;

// コライダーをシーンの当たり判定チェッカーに追加するクラス
// コンポーネント内から追加したかった為、シングルトンクラス
class CollisionRegister : public Singleton_Base<CollisionRegister>
{
	friend class Singleton_Base<CollisionRegister>;

	CollisionChecker* pCollisionChecker;	// コライダーの登録対象の当たり判定チェッカー

	CollisionRegister() : pCollisionChecker(nullptr) {};
	~CollisionRegister() {};
public:
	// 当たり判定チェッカークラスをセット
	void SetCollisionChecker(CollisionChecker& _pCollisionChecker);	
	// 当たり判定チェッカークラスに追加する
	void AddCollider(CP_Collider& _collider);
	// 当たり判定チェッカークラス
	void PopCollider(CP_Collider& _collider);
};


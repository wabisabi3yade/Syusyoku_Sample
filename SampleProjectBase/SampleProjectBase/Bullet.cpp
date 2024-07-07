#include "pch.h"
#include "Bullet.h"
#include "BulletMove.h"
#include "CP_SphereCollider.h"

Bullet::Bullet()
{
	name = "Bullet";

	// コンポーネントアタッチ
	AddComponent<BulletMove>();	// 弾移動
	AddComponent<CP_SphereCollider>();	// 球当たり判定
}

Bullet::~Bullet()
{
}

#include "pch.h"
#include "Bullet.h"
#include "BulletMove.h"
#include "SphereCollider.h"

Bullet::Bullet()
{
	name = "Bullet";

	// コンポーネントアタッチ
	AddComponent<BulletMove>();	// 弾移動
	AddComponent<SphereCollider>();	// 球当たり判定
}

Bullet::~Bullet()
{
}

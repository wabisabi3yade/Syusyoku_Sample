#include "pch.h"
#include "PlayerShot.h"
#include "Bullet.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;
using namespace SceneFunction;

void PlayerShot::Init()
{
	name = typeid(PlayerShot).name();
	bullet = std::make_unique<Bullet>();
}

void PlayerShot::Shot()
{
	// 弾発射方向を取得（Y成分0）
	Vector3 shotVec = GetTransform().Forward();
	shotVec.y = 0.0f;
	shotVec.Normalize();

	Vector3 shotPos = GetTransform().position + shotVec * shotDistance;

	// 弾生成
	Bullet* createBullet = static_cast<Bullet*>(&CreateObject(*bullet, shotPos));
}

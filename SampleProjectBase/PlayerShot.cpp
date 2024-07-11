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
	// �e���˕������擾�iY����0�j
	Vector3 shotVec = GetTransform().Forward();
	shotVec.y = 0.0f;
	shotVec.Normalize();

	Vector3 shotPos = GetTransform().position + shotVec * shotDistance;

	// �e����
	Bullet* createBullet = static_cast<Bullet*>(&Object::Instantiate(*bullet, shotPos));
}

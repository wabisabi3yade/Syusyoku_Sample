#include "pch.h"
#include "BossGroundMove.h"
#include "GameObject.h"

BossGroundMove::BossGroundMove()
{
}

void BossGroundMove::UpdateBehavior()
{
}

void BossGroundMove::Move()
{
	using namespace DirectX::SimpleMath;

	float deltaTime = MainApplication::DeltaTime();
	Transform& myTransform = pActionController->GetBoss().GetTransform();
	Transform& playerTrans = pActionController->GetPlayer().GetTransform();

	// 正面方向に移動
	Vector3 movement = playerTrans.Forward() * currentSpeed * deltaTime;

	// 座標反映
	Vector3 pos = myTransform.GetPosition();
	pos += movement;
	myTransform.SetPosition(pos);
}


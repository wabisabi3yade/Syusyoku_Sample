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

	// ³–Ê•ûŒü‚ÉˆÚ“®
	Vector3 movement = playerTrans.Forward() * currentSpeed * deltaTime;

	// À•W”½‰f
	Vector3 pos = myTransform.GetPosition();
	pos += movement;
	myTransform.SetPosition(pos);
}


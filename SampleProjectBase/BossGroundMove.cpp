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

	// ���ʕ����Ɉړ�
	Vector3 movement = playerTrans.Forward() * currentSpeed * deltaTime;

	// ���W���f
	Vector3 pos = myTransform.GetPosition();
	pos += movement;
	myTransform.SetPosition(pos);
}


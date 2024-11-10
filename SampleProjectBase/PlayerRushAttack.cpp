#include "pch.h"
#include "PlayerRushAttack.h"
#include "GameObject.h"

PlayerRushAttack::PlayerRushAttack()
	: moveSpeed(10.0f), moveTime(1.0f), elapsedMoveTime(0.0f)
{
}

void PlayerRushAttack::OnStartBehavior()
{
	PlayerGroundAttack::OnStartBehavior();

	elapsedMoveTime = 0.0f;
}

void PlayerRushAttack::UpdateBehavior()
{
	using namespace DirectX::SimpleMath;

	PlayerGroundAttack::UpdateBehavior();

	float deltaTime = MainApplication::DeltaTime();
	Transform& playerTrans = pActionController->GetPlayer().GetGameObject().GetTransform();

	// ˆÚ“®‹——£‚ð‹‚ß‚é
	Vector3 movement = playerTrans.Forward() * moveSpeed * deltaTime;

	// À•W”½‰f
	Vector3 pos = playerTrans.GetPosition();
	pos += movement;
	playerTrans.SetPosition(pos);

	elapsedMoveTime += deltaTime;
}

void PlayerRushAttack::TransitionCheckUpdate()
{
	if (elapsedMoveTime > moveTime)
		ChangeState(PlayerState::Idle);
}

void PlayerRushAttack::ImGuiDebug()
{
	ImGui::DragFloat("Speed", &moveSpeed, 0.01f, 0.0f, 1000.0f);
	ImGui::DragFloat("Time", &moveTime, 0.01f, 0.0f, 1000.0f);
}

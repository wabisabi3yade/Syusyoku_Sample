#include "pch.h"
#include "PlayerJumpState.h"

PlayerJumpState::PlayerJumpState() :
	jumpPower(10.0f),
	crouchTime(0.3f),
	stateElapsedTime(0.0f),
	isAlreadyJump(false)
{
}

PlayerJumpState::~PlayerJumpState()
{
}

void PlayerJumpState::OnStartBehavior()
{
	stateElapsedTime = 0.0f;
	isAlreadyJump = false;
}

void PlayerJumpState::UpdateBehavior()
{
	if (isAlreadyJump) return;

	stateElapsedTime += DeltaTime();

	// かがむ時間を過ぎると上に飛ぶ
	if (stateElapsedTime > crouchTime)
	{
		OnBeginJump();
	}
}

void PlayerJumpState::OnEndBehavior()
{
}

void PlayerJumpState::OnBeginJump()
{
	isAlreadyJump = true;

	// 上に飛ぶ
	GetRB().AddImpulse(Vec3::Up * jumpPower);
}

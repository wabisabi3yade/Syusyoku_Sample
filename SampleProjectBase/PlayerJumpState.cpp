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

	// Ç©Ç™Çﬁéûä‘ÇâﬂÇ¨ÇÈÇ∆è„Ç…îÚÇ‘
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

	// è„Ç…îÚÇ‘
	GetRB().AddImpulse(Vec3::Up * jumpPower);
}

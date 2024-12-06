#include "pch.h"
#include "PlayerJumpState.h"

constexpr auto JUMPTRIGGER_ANIMPARAM("jumpTrigger");

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
	// �W�����v�g���K�[������
	GetAnimation()->SetTrigger(JUMPTRIGGER_ANIMPARAM);

	stateElapsedTime = 0.0f;
	isAlreadyJump = false;
}

void PlayerJumpState::UpdateBehavior()
{
	if (isAlreadyJump) return;

	stateElapsedTime += DeltaTime();

	// �����ގ��Ԃ��߂���Ə�ɔ��
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

	// ��ɔ��
	GetRB().AddImpulse(Vec3::Up * jumpPower);
}

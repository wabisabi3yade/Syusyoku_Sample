#include "pch.h"
#include "PlayerIdleState.h"

PlayerIdleState::PlayerIdleState()
	: leaveElapsedTime(0.0f), specialIdleTransTiime(3.0f)
{
}

void PlayerIdleState::OnStartBehavior()
{
	leaveElapsedTime = 0.0f;
}

void PlayerIdleState::UpdateBehavior()
{
	// �ړ��X�e�[�g�ɑJ�ڂł���Ȃ�
	if (IsCanMoveTransition())
	{
		ChangeState(StateType::Move);
	}

	ProgressLeaveElapse();
}

void PlayerIdleState::OnEndBehavior()
{
}

void PlayerIdleState::ProgressLeaveElapse()
{
	/*leaveElapsedTime += MainApplication::DeltaTime();

	if (leaveElapsedTime > specialIdleTransTiime)
	{

	}*/
}

bool PlayerIdleState::IsCanMoveTransition()
{
	using namespace DirectX::SimpleMath;

	// ���͒l��0�Ȃ�
	Vector2 inputVal = GameInput::GetInstance()->GetValue(GameInput::ValueType::Player_Move);
	if (inputVal.Length() <= Mathf::epsilon)return false;

	return true;
}

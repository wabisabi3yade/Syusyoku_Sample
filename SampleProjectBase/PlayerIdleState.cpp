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
	// 移動ステートに遷移できるなら
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

	// 入力値が0なら
	Vector2 inputVal = GameInput::GetInstance()->GetValue(GameInput::ValueType::Player_Move);
	if (inputVal.Length() <= Mathf::epsilon)return false;

	return true;
}

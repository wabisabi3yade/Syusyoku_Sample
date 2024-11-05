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
	ProgressLeaveElapse();
}

void PlayerIdleState::OnEndBehavior()
{
}

void PlayerIdleState::TransitionCheckUpdate()
{
	// 移動ステートに遷移できるなら
	if (pPlayerInput->GetButton(GameInput::ButtonType::Player_RockOn) && IsCanMoveTransition())
	{
		ChangeState(PlayerState::TargetMove);
	}
	else if (IsCanMoveTransition())
	{
		ChangeState(PlayerState::Move);
	}
	// 攻撃ボタン
	else if(pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
	{
		ChangeState(PlayerState::Attack11);
	}
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

#include "pch.h"
#include "PlayerIdleState.h"
#include "CP_Player.h"

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

	// アニメーションのブレンド割合をセット
	pAnimation->SetFloat(SPEEDRATIO_PARAMNAME, 0.0f);
}

void PlayerIdleState::OnEndBehavior()
{
}

void PlayerIdleState::TransitionCheckUpdate()
{
	// 移動ステートに遷移できるなら
	if (IsCanMoveTransition())
	{
		if(pActionController->GetIsTargeting())
			ChangeState(PlayerState::TargetMove);
		else
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
}

bool PlayerIdleState::IsCanMoveTransition()
{
	using namespace DirectX::SimpleMath;

	// 入力値が0なら
	Vector2 inputVal = GetInputLeftStick();
	if (inputVal.Length() <= Mathf::epsilon)return false;

	return true;
}

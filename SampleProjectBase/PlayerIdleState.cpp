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

	// �A�j���[�V�����̃u�����h�������Z�b�g
	pAnimation->SetFloat(SPEEDRATIO_PARAMNAME, 0.0f);
}

void PlayerIdleState::OnEndBehavior()
{
}

void PlayerIdleState::TransitionCheckUpdate()
{
	// �ړ��X�e�[�g�ɑJ�ڂł���Ȃ�
	if (IsCanMoveTransition())
	{
		if(pActionController->GetIsTargeting())
			ChangeState(PlayerState::TargetMove);
		else
			ChangeState(PlayerState::Move);
	}
	// �U���{�^��
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

	// ���͒l��0�Ȃ�
	Vector2 inputVal = GetInputLeftStick();
	if (inputVal.Length() <= Mathf::epsilon)return false;

	return true;
}

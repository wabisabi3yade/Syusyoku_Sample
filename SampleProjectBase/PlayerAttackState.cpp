#include "pch.h"
#include "PlayerAttackState.h"

#include "PlayerActionController.h"
#include "PlayerAnimController.h"

PlayerAttackState::PlayerAttackState(PlayerActionController& _pController) 
	: PlayerActState_Base(_pController)
{
}

void PlayerAttackState::Init()
{
	using enum PlayerAnimController::AnimType;
	ChangeAnimation((u_int)Attack);
}

void PlayerAttackState::Update()
{
}

void PlayerAttackState::Terminal()
{
}

void PlayerAttackState::TransitionCheck()
{
	using enum PlayerActionController::State;
	u_int stateNum = static_cast<u_int>(Move);

	ButtonChangeState(GameInput::ButtonType::Player_Attack, stateNum);
}

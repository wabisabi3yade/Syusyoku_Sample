#include "pch.h"
#include "PlayerActState_Base.h"

#include "PlayerActionController.h"
#include "PlayerAnimController.h"

void PlayerActState_Base::ButtonChangeState(GameInput::ButtonType _buttonType, u_int _nextState)
{
	using enum PlayerActionController::State;

	assert(_nextState < static_cast<u_int>(MaxNum) && "範囲外です");

	GameInput* input = GameInput::GetInstance();

	PlayerActionController::State next = static_cast<PlayerActionController::State>(_nextState);

	if (input->GetButtonDown(_buttonType))
	{
		pActionController->TransitionState(next);
	}
}

void PlayerActState_Base::ChangeAnimation(u_int _animState)
{
	pAnimController->ChangeAnimationByState(
		static_cast<PlayerAnimController::AnimType>(_animState)
	);
}

PlayerActState_Base::PlayerActState_Base(PlayerActionController& _pController)
	: pActionController(&_pController), pPlayerObject(nullptr), pAnimController(nullptr)
{
}

void PlayerActState_Base::UpdateBase()
{
	// 各アクション更新
	Update();

	// 遷移チェック
	TransitionCheck();
}

void PlayerActState_Base::SetPlayerObject(GameObject& _playerObj)
{
	pPlayerObject = &_playerObj;
}

void PlayerActState_Base::SetAnimController(PlayerAnimController& _pController)
{
	pAnimController = &_pController;
}

#include "pch.h"
#include "PlayerAnimObserver.h"

#include "PlayerActionController.h"
#include "PlayerAnimController.h"

PlayerAnimObserver::PlayerAnimObserver(PlayerActionController& _pActionController)
	: pActionController(&_pActionController)
{
}

void PlayerAnimObserver::OnFinishAnimation(u_int _animState)
{
	PlayerAnimController::AnimType state = static_cast<PlayerAnimController::AnimType>(_animState);

	//switch (state)
	//{
	//case PlayerAnimController::AnimType::Attack:
	//	pActionController->TransitionState(PlayerActionController::State::Move);
	//	break;

	//default:
	//	break;
	//}
}


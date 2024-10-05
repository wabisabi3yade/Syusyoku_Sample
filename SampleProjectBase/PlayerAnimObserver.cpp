#include "pch.h"
#include "PlayerAnimObserver.h"

#include "PlayerActionController.h"
PlayerAnimObserver::PlayerAnimObserver(PlayerActionController& _pActionController)
	: pActionController(&_pActionController)
{
}

void PlayerAnimObserver::OnFinishAnimation(u_int _animState)
{

	//switch (state)
	//{
	//case PlayerAnimController::AnimType::Attack:
	//	pActionController->TransitionState(PlayerActionController::State::Move);
	//	break;

	//default:
	//	break;
	//}
}


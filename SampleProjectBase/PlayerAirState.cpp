#include "pch.h"
#include "PlayerAirState.h"
#include "PlayerAirActionController.h"

void PlayerAirState::TransitionCheckUpdate()
{
}

PlayerAirActionController& PlayerAirState::CastAirController()
{
	return static_cast<PlayerAirActionController&>(*pActionController);
}

void PlayerAirState::ChangeState(PlayerState _nextState, bool _isForce)
{
	CastAirController().ChangeAirState(_nextState, _isForce);
}

void PlayerAirState::ImGuiDebug()
{
	PlayerActState_Base::ImGuiDebug();
}

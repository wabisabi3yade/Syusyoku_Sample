#include "pch.h"
#include "PlayerAirState.h"
#include "PlayerAirActionController.h"

void PlayerAirState::OnStart()
{
	PlayerActState_Base::OnStart();

	OnStartBehavior();
}

void PlayerAirState::Update()
{
	InputUpdate();

	PlayerActState_Base::Update();

	UpdateBehavior();

	TransitionCheckUpdate();
}

void PlayerAirState::OnEnd()
{
	PlayerActState_Base::OnEnd();

	OnEndBehavior();
}

void PlayerAirState::TransitionCheckUpdate()
{
	PlayerActState_Base::OnEnd();

	OnEndBehavior();
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

nlohmann::json PlayerAirState::Save()
{
	return PlayerActState_Base::Save();
}

void PlayerAirState::Load(const nlohmann::json& _data)
{
	PlayerActState_Base::Load(_data);
}

void PlayerAirState::InputUpdate()
{
}

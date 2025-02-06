#include "pch.h"
#include "PlayerAirState.h"
#include "PlayerAirActionController.h"

namespace HashiTaku
{
	bool PlayerAirState::OnDamage(AttackInformation& _attackInfo)
	{
		return true;
	}

	void PlayerAirState::OnStart()
	{
		PlayerActState_Base::OnStart();

		// äJénèàóù
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
	}

	PlayerAirActionController& PlayerAirState::GetAirController()
	{
		return GetDeliverActionController<PlayerAirActionController>();
	}

	void PlayerAirState::ChangeState(PlayerState _nextState, bool _isForce)
	{
		GetAirController().ChangeAirState(_nextState, _isForce);
	}

	void PlayerAirState::ImGuiDebug()
	{
		PlayerActState_Base::ImGuiDebug();
	}

	json PlayerAirState::Save()
	{
		return PlayerActState_Base::Save();
	}

	void PlayerAirState::Load(const json& _data)
	{
		PlayerActState_Base::Load(_data);
	}

	void PlayerAirState::InputUpdate()
	{
		if (!GetAirController().GetCanInput()) return;

		using enum GameInput::ButtonType;
	}
}
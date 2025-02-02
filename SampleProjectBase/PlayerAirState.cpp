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
		// �����ł����Ԃ��H
		if (!CanDoProcess()) return;

		PlayerActState_Base::OnStart();

		// �J�n����
		OnStartBehavior();
	}

	void PlayerAirState::Update()
	{
		// �����ł����Ԃ��H
		if (!CanDoProcess()) return;

		InputUpdate();

		PlayerActState_Base::Update();

		UpdateBehavior();

		TransitionCheckUpdate();
	}

	void PlayerAirState::OnEnd()
	{
		// �����ł����Ԃ��H
		if (!CanDoProcess()) return;

		PlayerActState_Base::OnEnd();

		OnEndBehavior();
	}

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
		if (!pActionController->GetCanInput()) return;

		using enum GameInput::ButtonType;
	}
}
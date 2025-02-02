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
		// 処理できる状態か？
		if (!CanDoProcess()) return;

		PlayerActState_Base::OnStart();

		// 開始処理
		OnStartBehavior();
	}

	void PlayerAirState::Update()
	{
		// 処理できる状態か？
		if (!CanDoProcess()) return;

		InputUpdate();

		PlayerActState_Base::Update();

		UpdateBehavior();

		TransitionCheckUpdate();
	}

	void PlayerAirState::OnEnd()
	{
		// 処理できる状態か？
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
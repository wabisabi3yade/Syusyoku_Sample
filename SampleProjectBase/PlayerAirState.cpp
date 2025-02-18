#include "pch.h"
#include "PlayerAirState.h"
#include "PlayerAirActionController.h"

namespace HashiTaku
{
	PlayerAirState::PlayerAirState() : isApplyDownForce(true)
	{
	}

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
		InputStateUpdate();

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

	void PlayerAirState::SetIsApplyDownForce(bool _isDownForce)
	{
		isApplyDownForce = _isDownForce;
	}

	PlayerAirActionController& PlayerAirState::GetAirController()
	{
		return GetDeliverActionController<PlayerAirActionController>();
	}

	bool PlayerAirState::GetIsApplyDownForce() const
	{
		return isApplyDownForce;
	}

	void PlayerAirState::ChangeState(PlayerState _nextState, bool _isForce)
	{
		GetAirController().ChangeAirState(_nextState, _isForce);
	}

	void PlayerAirState::ImGuiDebug()
	{
		PlayerActState_Base::ImGuiDebug();

		ImGuiMethod::LineSpaceSmall();
		ImGui::Text("Air");
		ImGui::Checkbox("ApplyDownForce", &isApplyDownForce);
	}

	json PlayerAirState::Save()
	{
		return PlayerActState_Base::Save();
	}

	void PlayerAirState::Load(const json& _data)
	{
		PlayerActState_Base::Load(_data);
	}

	void PlayerAirState::InputStateUpdate()
	{
		if (!GetAirController().GetCanInput()) return;

		using enum GameInput::ButtonType;
	}
}
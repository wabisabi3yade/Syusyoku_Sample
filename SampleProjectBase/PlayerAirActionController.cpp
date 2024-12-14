#include "pch.h"
#include "PlayerAirActionController.h"
#include "PlayerAction.h"

#include "PlayerAirMove.h"
#include "PlayerAirAttack.h"

PlayerAirActionController::PlayerAirActionController(PlayerAction& _pAction, CP_Player& _player) :
	PlayerActionController_Base(_pAction, _player, "playerAirController"),
	downForcePower(0.0f),
	isDownForce(true)
{
	place = ActionPlace::Air;

	// 行動クラスを生成
	using enum AirState;
	CreateState<PlayerAirMove>(Move);
	//CreateState<PlayerAirAttack>(Attack11);

	// デフォルト状態をセット
	SetDefaultNode(static_cast<int>(Move));
}

void PlayerAirActionController::Update()
{
	// 下向きに力を加える
	AddDownForce();

	PlayerActionController_Base::Update();
}

bool PlayerAirActionController::ChangeAirState(const AirState& _nextActionState, bool _isForce)
{
	return ChangeState(static_cast<int>(_nextActionState), _isForce);
}

void PlayerAirActionController::SetIsDownForce(bool _isDown)
{
	isDownForce = _isDown;
}

std::string PlayerAirActionController::GetStateStr(int _stateId)
{
	AirState state = static_cast<AirState>(_stateId);
	std::string str = std::string(magic_enum::enum_name<AirState>(state));

	return str;
}

int PlayerAirActionController::GetStateId(const std::string& _stateName)
{
	auto state = magic_enum::enum_cast<AirState>(_stateName);
	if (!state.has_value())
	{
		assert(!"正常に状態を取得できません");
		return -1;
	}

	return static_cast<int>(state.value());
}

nlohmann::json PlayerAirActionController::Save()
{
	auto data = PlayerActionController_Base::Save();

	data["downForcePower"] = downForcePower;

	return data;
}

void PlayerAirActionController::Load(const nlohmann::json& _data)
{
	PlayerActionController_Base::Load(_data);
	HashiTaku::LoadJsonFloat("downForcePower", downForcePower, _data);
}

void PlayerAirActionController::AddDownForce()
{
	if (!isDownForce) return;

	// 下向きに力を加える
	GetRB()->AddForce(Vec3::Up * downForcePower);

	HASHI_DEBUG_LOG("down");
}

void PlayerAirActionController::ImGuiDebug()
{
	PlayerActionController_Base::ImGuiDebug();

	ImGui::DragFloat("DownPower", &downForcePower, 0.01f);
}

#include "pch.h"
#include "PlayerAirActionController.h"

#include "PlayerAirMove.h"

PlayerAirActionController::PlayerAirActionController(PlayerAction& _pAction, CP_Player& _player) :
	PlayerActionController_Base(_pAction, _player, "playerAirController"),
	downForcePower(0.0f)
{
	place = ActionPlace::Air;

	// �s���N���X�𐶐�
	using enum AirState;
	CreateState<PlayerAirMove>(Move);

	// �f�t�H���g��Ԃ��Z�b�g
	SetDefaultNode(static_cast<int>(Move));
}

void PlayerAirActionController::Update()
{
	PlayerActionController_Base::Update();

	// �������ɗ͂�������
	AddDownForce();
}

bool PlayerAirActionController::ChangeAirState(const AirState& _nextActionState, bool _isForce)
{
	return ChangeState(static_cast<int>(_nextActionState), _isForce);
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
		assert(!"����ɏ�Ԃ��擾�ł��܂���");
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
	// �������ɗ͂�������
	GetRB()->AddForce(Vec3::Up * downForcePower);
}

void PlayerAirActionController::ImGuiDebug()
{
	PlayerActionController_Base::ImGuiDebug();

	ImGui::DragFloat("DownPower", &downForcePower, 0.01f);
}

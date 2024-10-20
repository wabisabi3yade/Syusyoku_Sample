#include "pch.h"
#include "PlayerAttackState.h"

#include "PlayerActionController.h"

PlayerAttackState::PlayerAttackState()
	: PlayerActState_Base(StateType::Attack)
{
}

void PlayerAttackState::OnStart()
{
}

void PlayerAttackState::Update()
{
}

void PlayerAttackState::OnEnd()
{
}

void PlayerAttackState::ImGuiSetting()
{
}

nlohmann::json PlayerAttackState::Save()
{
	auto data = PlayerActState_Base::Save();
	return data;
}

void PlayerAttackState::Load(const nlohmann::json& _data)
{
	PlayerActState_Base::Load(_data);
}

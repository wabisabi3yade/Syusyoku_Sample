#include "pch.h"
#include "BossActState_Base.h"

BossActState_Base::BossActState_Base()
	: stateType(BossState::None), pActionController(nullptr)
{
}

void BossActState_Base::Init(BossState _stateType, BossActionController& _actController)
{
	stateType = _stateType;
	pActionController = &_actController;
}

void BossActState_Base::OnStart()
{
	EnemyActState_Base::OnStart();

	OnStartBehavior();
}

void BossActState_Base::Update()
{
	EnemyActState_Base::Update();

	UpdateBehavior();
}

void BossActState_Base::OnEnd()
{
	EnemyActState_Base::OnEnd();

	OnEndBehavior();
}

nlohmann::json BossActState_Base::Save()
{
	return nlohmann::json();
}

void BossActState_Base::Load(const nlohmann::json& _data)
{
}

void BossActState_Base::ImGuiSetting()
{
}

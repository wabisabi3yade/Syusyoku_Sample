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

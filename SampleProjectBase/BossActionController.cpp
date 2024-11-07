#include "pch.h"
#include "BossActionController.h"

BossActionController::BossActionController(CP_Character& _boss)
	: EnemyActionController(_boss, "bossActCon")
{
}

void BossActionController::Init(CP_Animation& _animationController)
{
	EnemyActionController::Init(_animationController);
}

void BossActionController::Update()
{
	EnemyActionController::Update();
}

bool BossActionController::ChangeState(BossActState_Base::BossState _nextState)
{
	return ChangeNode(static_cast<int>(_nextState));
}


#include "pch.h"
#include "BossActState_Base.h"
#include "BossActionController.h"
#include "CP_Boss.h"
#include "CP_Player.h"

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
	
	TransitionCheckUpdate();
}

void BossActState_Base::OnEnd()
{
	EnemyActState_Base::OnEnd();

	OnEndBehavior();
}

void BossActState_Base::OnDamage()
{
	// ‚Ì‚¯‚¼‚ç‚È‚¢‚È‚çI‚¦‚é
	if (!pActionController->GetCanKnock()) return;

	// ‚Ì‚¯‚¼‚é‚æ‚¤‚É
	ChangeState(BossState::Damage_Small, true);
}

nlohmann::json BossActState_Base::Save()
{
	return nlohmann::json();
}

void BossActState_Base::Load(const nlohmann::json& _data)
{
}

void BossActState_Base::ChangeState(BossState _nextState, bool _isForce)
{
	pActionController->ChangeState(_nextState, _isForce);
}

Transform& BossActState_Base::GetBossTransform()
{
	return pActionController->GetBoss().GetTransform();
}

Transform& BossActState_Base::GetPlayerTransform()
{
	return pActionController->GetPlayer().GetTransform();
}

CP_Animation* BossActState_Base::GetAnimation()
{
	return pActionController->GetAnimation();
}

CP_RigidBody* BossActState_Base::GetRB()
{
	return pActionController->GetRB();
}

float BossActState_Base::DeltaTime() const
{
	return pActionController->GetCharacter().DeltaTime();
}

void BossActState_Base::ImGuiDebug()
{
}

void BossActState_Base::CommmonCheckTransition()
{
}

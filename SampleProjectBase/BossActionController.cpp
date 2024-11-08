#include "pch.h"
#include "BossActionController.h"
#include "CP_Boss.h"
#include "CP_BattleManager.h"

#include "BossGroundMove.h"

BossActionController::BossActionController(CP_Boss& _boss)
	: EnemyActionController(_boss, "bossActCon"), pPlayerObject(nullptr)
{
}

void BossActionController::Init(CP_Animation& _animationController)
{
	EnemyActionController::Init(_animationController);

	// プレイヤーを取得する
	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pPlayerObject = pBattle->GetPlayerObject();
	}

	// ステート作成
	using enum BossActState_Base::BossState;
	CreateState<BossGroundMove>(Run);

	// デフォルトノード設定
	SetDefaultNode(static_cast<int>(Run));
}

void BossActionController::Update()
{
	if (!IsCanBossUpdate()) return;

	EnemyActionController::Update();
}

bool BossActionController::ChangeState(BossActState_Base::BossState _nextState)
{
	return ChangeNode(static_cast<int>(_nextState));
}

CP_Boss& BossActionController::GetBoss()
{
	return static_cast<CP_Boss&>(GetCharacter());
}

GameObject& BossActionController::GetPlayer()
{
	assert(pPlayerObject && "プレイヤーが設定されていません");

	return *pPlayerObject;
}

bool BossActionController::IsCanBossUpdate()
{
#ifdef EDIT
	if (!pPlayerObject) return false;
#endif

	return true;
}


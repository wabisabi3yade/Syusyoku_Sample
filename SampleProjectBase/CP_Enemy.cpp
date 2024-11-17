#include "pch.h"
#include "CP_Enemy.h"
#include "CP_BattleManager.h"

CP_Enemy::CP_Enemy() : pTargeter(nullptr)
{
	SetEnemyName("Boss");
}

const std::string& CP_Enemy::GetEnemyName() const
{
	return enemyName;
}

const DirectX::SimpleMath::Vector3& CP_Enemy::GetWorldPosByTargetObj() const
{
	return GetTransform().GetPosition();
}

void CP_Enemy::GetTargeter(IObjectTargeter& _targeter)
{
	pTargeter = &_targeter;
}

void CP_Enemy::OnDeathNotifyToTargeter()
{
	// 伝える
	if (pTargeter)
		pTargeter->UpdateDeathNotify(*this);
}

void CP_Enemy::Awake()
{
	// バトルマネジャーがあるなら敵を追加する
	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pBattle->AddEnemy(*this);
	}
}

void CP_Enemy::OnDestroy()
{
	OnDeathNotifyToTargeter();

	// バトルマネジャーがあるなら自身を削除する
	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pBattle->RemoveEnemy(*this);
	}
}

void CP_Enemy::OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo)
{
	if (GetIsInvicible()) return;

	currentHP -= _attackInfo.GetDamageValue();

	// ヒットストップをする
	BeginHitStop(_attackInfo.GetHitStopFlame());
}

void CP_Enemy::OnDeathBehavior()
{
	HASHI_DEBUG_LOG("敵死んだ");
}

void CP_Enemy::SetEnemyName(const std::string& _enemyName)
{
	enemyName = _enemyName;
}

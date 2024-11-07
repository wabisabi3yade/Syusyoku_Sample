#include "pch.h"
#include "CP_Enemy.h"

CP_Enemy::CP_Enemy()
	: enemyName("enemy_default")
{
}

CP_Enemy::CP_Enemy(const std::string& _enemyName)
	: enemyName(_enemyName)
{
}

const std::string& CP_Enemy::GetEnemyName() const
{
	return enemyName;
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

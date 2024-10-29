#include "pch.h"
#include "CP_Enemy.h"

CP_Enemy::CP_Enemy()
{
}

void CP_Enemy::OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo)
{
	currentHP -= _attackInfo.GetDamageValue();
}

void CP_Enemy::OnDeathBehavior()
{
	HASHI_DEBUG_LOG("“GŽ€‚ñ‚¾");
}

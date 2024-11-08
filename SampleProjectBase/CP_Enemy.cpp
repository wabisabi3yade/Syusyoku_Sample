#include "pch.h"
#include "CP_Enemy.h"

CP_Enemy::CP_Enemy()
{
	SetEnemyName("Boss");
}

const std::string& CP_Enemy::GetEnemyName() const
{
	return enemyName;
}

void CP_Enemy::OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo)
{
	if (GetIsInvicible()) return;

	currentHP -= _attackInfo.GetDamageValue();

	// �q�b�g�X�g�b�v������
	BeginHitStop(_attackInfo.GetHitStopFlame());
}

void CP_Enemy::OnDeathBehavior()
{
	HASHI_DEBUG_LOG("�G����");
}

void CP_Enemy::SetEnemyName(const std::string& _enemyName)
{
	enemyName = _enemyName;
}

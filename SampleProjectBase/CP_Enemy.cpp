#include "pch.h"
#include "CP_Enemy.h"
#include "CP_BattleManager.h"
#include "CP_HitStopManager.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	CP_Enemy::CP_Enemy()
	{
		SetEnemyName("Boss");
	}

	const std::string& CP_Enemy::GetEnemyName() const
	{
		return enemyName;
	}

	const DXSimp::Vector3& CP_Enemy::GetWorldPos() const
	{
		return GetTransform().GetPosition();
	}

	void CP_Enemy::SetTargeter(IObjectTargeter& _targeter)
	{
		auto itr = std::find(pTargeters.begin(), pTargeters.end(), &_targeter);
		if (itr != pTargeters.end())
		{
			HASHI_DEBUG_LOG("�^�[�Q�b�g���d�����܂���");
			return;
		}

		pTargeters.push_back(&_targeter);
	}

	void CP_Enemy::RemoveTargeter(IObjectTargeter& _targeter)
	{
		pTargeters.remove(&_targeter);
	}

	void CP_Enemy::OnRemoveNotifyToTargeter()
	{
		for (auto& targeter : pTargeters)
			targeter->RemoveNotify(*this);
	}

	void CP_Enemy::Awake()
	{
		// �o�g���}�l�W���[������Ȃ�G��ǉ�����
		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pBattle->AddEnemy(*this);
		}
	}

	void CP_Enemy::OnDestroy()
	{
		OnRemoveNotifyToTargeter();

		// �o�g���}�l�W���[������Ȃ玩�g���폜����
		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pBattle->RemoveEnemy(*this);
		}
	}

	bool CP_Enemy::OnDamageBehavior(const AttackInformation& _attackInfo,
		const DXSimp::Vector3& _attackerPos)
	{
		// HP�����炷
		DecadeHp(_attackInfo.GetDamageValue());

		return true;
	}

	void CP_Enemy::OnTakeDamage(const AttackInformation& _attackInfo, const DXSimp::Vector3& _contactPos)
	{
		CP_Character::OnTakeDamage(_attackInfo, _contactPos);

		// �q�b�g�X�g�b�v
		if (CP_HitStopManager* pHitStop = CP_HitStopManager::GetInstance())
		{
			pHitStop->HitStopBegin(_attackInfo.GetHitStopFlame());
		}

		// �p�b�h�U��
		InSceneSystemManager::GetInstance()->GetInput().
			BeginVibration(_attackInfo.GetPadShakePower(), _attackInfo.GetPadShakeTime());
	}

	void CP_Enemy::SetEnemyName(const std::string& _enemyName)
	{
		enemyName = _enemyName;
	}
}
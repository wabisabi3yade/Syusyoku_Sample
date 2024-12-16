#include "pch.h"
#include "CP_Enemy.h"
#include "CP_BattleManager.h"

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

	const DirectX::SimpleMath::Vector3& CP_Enemy::GetWorldPos() const
	{
		return GetTransform().GetPosition();
	}

	void CP_Enemy::SetTargeter(IObjectTargeter& _targeter)
	{
		auto itr = std::find(pTargeters.begin(), pTargeters.end(), &_targeter);
		if (itr != pTargeters.end())
		{
			HASHI_DEBUG_LOG("ターゲットが重複しました");
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
		// バトルマネジャーがあるなら敵を追加する
		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pBattle->AddEnemy(*this);
		}
	}

	void CP_Enemy::OnDestroy()
	{
		OnRemoveNotifyToTargeter();

		// バトルマネジャーがあるなら自身を削除する
		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pBattle->RemoveEnemy(*this);
		}
	}

	void CP_Enemy::OnDamageBehavior(const AttackInformation& _attackInfo,
		const DirectX::SimpleMath::Vector3& _attackerPos)
	{
		if (GetIsInvicible()) return;

		// HPを減らす
		DecadeHp(_attackInfo.GetDamageValue());

		// ヒットストップをする
		BeginHitStop(_attackInfo.GetHitStopFlame());
	}

	void CP_Enemy::SetEnemyName(const std::string& _enemyName)
	{
		enemyName = _enemyName;
	}
}
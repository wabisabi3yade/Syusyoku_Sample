#pragma once
#include "CP_Character.h"

class CP_Enemy : public HashiTaku::CP_Character
{
	/// @brief 敵の名称
	std::string enemyName;

public:
	CP_Enemy();
	CP_Enemy(const std::string& _enemyName);
	~CP_Enemy() {}

	const std::string& GetEnemyName() const;

private:
	/// @brief 各クラスのダメージを受けたときの処理
		/// @param _attackInfo 攻撃情報
	virtual void OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo);

	/// @brief 各クラスのダメージを受けたときの処理
	/// @param _attackInfo 攻撃情報
	virtual void OnDeathBehavior();
};


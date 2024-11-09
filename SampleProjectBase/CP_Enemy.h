#pragma once
#include "CP_Character.h"

class CP_Enemy : public CP_Character
{
	/// @brief 敵の名称
	std::string enemyName;

public:
	CP_Enemy();
	virtual ~CP_Enemy() {}

	/// @brief 敵名を取得
	/// @return 敵名
	const std::string& GetEnemyName() const;
private:
	/// @brief 各クラスのダメージを受けたときの処理
	/// @param _attackInfo 攻撃情報
	void OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo) override;

	/// @brief 各クラスのダメージを受けたときの処理
	/// @param _attackInfo 攻撃情報
	void OnDeathBehavior() override;

protected:
	void Awake() override;
	void OnDestroy() override;

	void SetEnemyName(const std::string& _enemyName);
};


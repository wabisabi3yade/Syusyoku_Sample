#pragma once
#include "Component.h"
#include "AttackInformation.h"
#include "IDamageable.h"

/// @brief 武器コンポーネント
class CP_Weapon : public Component
{
	/// @brief 攻撃情報
	HashiTaku::AttackInformation atkInfomation;

	/// @brief 攻撃判定コリジョン
	bool isAttackCollision;
public:
	CP_Weapon();
	~CP_Weapon() {}

	// コンポーネント共通
	void OnCollisionEnter(const HashiTaku::CollisionInfo& _otherColInfo);

	void SetAttackInfo(const HashiTaku::AttackInformation& _attackInformation);

	/// @brief 武器の攻撃判定コリジョン
	/// @param _isAttackCollision 
	void SetIsAttackCollision(bool _isAttackCollision);

private:
	/// @brief 攻撃処理
	/// @param _damager 攻撃与える対象
	void OnAttack(HashiTaku::IDamageable& _damager);
};




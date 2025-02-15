#pragma once
#include "AttackInformation.h"

namespace HashiTaku
{
	/// @brief ダメージ情報
	struct DamageInfo
	{
		/// @brief 攻撃情報
		AttackInformation* pAttackInformation;

		/// @brief 攻撃の方向
		DXSimp::Vector3 attackVector;

		/// @brief 攻撃衝突地点
		DXSimp::Vector3 contactPos;
	};

	/// @brief ダメージを受ける側のインターフェース
	class IDamageable
	{

	public:
		IDamageable() {}
		virtual ~IDamageable() {}

		/// @brief ダメージを受けたときの処理
		/// @param _damageInfo ダメージ情報
		virtual void OnDamage(const DamageInfo& _damageInfo) = 0;

		/// @brief 死んだときの処理
		virtual void OnDeath() = 0;
	};
}

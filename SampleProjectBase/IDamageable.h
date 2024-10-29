#pragma once
#include "AttackInformation.h"

namespace HashiTaku
{
	/// @brief ダメージを受ける側のインターフェース
	class IDamageable
	{

	public:
		IDamageable() {}
		virtual ~IDamageable() {}

		/// @brief ダメージを受けたときの処理
		/// @param _attackInfo 攻撃情報
		virtual void OnDamage(const AttackInformation& _attackInfo) = 0;

		/// @brief 死んだときの処理
		virtual void OnDeath() = 0;
	};
}

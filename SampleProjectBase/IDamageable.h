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
		/// @param _attackerPos 攻撃したものの座標
		/// @param _contactPos 衝突地点座標
		virtual void OnDamage(const AttackInformation& _attackInfo,
			const DXSimp::Vector3& _attackerPos,
			const DXSimp::Vector3& _contactPos) = 0;

		/// @brief 死んだときの処理
		virtual void OnDeath() = 0;
	};
}

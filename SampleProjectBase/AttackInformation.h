#pragma once

namespace HashiTaku
{
	/// @brief  攻撃の情報
	class AttackInformation
	{
		/// @brief 攻撃時のダメージ
		float atkDamage;

		/// @brief ヒットストップで止めるフレーム数
		u_int hitStopFlame;

	public:
		AttackInformation();
		AttackInformation(float _atkDamage, u_int _hitStopFlame);
		~AttackInformation() {}

		/// @brief ダメージ値を取得
		/// @return ダメージ値
		float GetDamageValue() const;

		/// @brief ヒットストップで止めるフレーム数を取得
		/// @return ヒットストップ数
		u_int GetHitStopFlame() const;
	};
}



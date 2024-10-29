#pragma once

namespace HashiTaku
{
	/// @brief  攻撃の情報
	class AttackInformation
	{
		/// @brief 攻撃時のダメージ
		float atkDamage;

	public:
		AttackInformation();
		AttackInformation(float _atkDamage);
		~AttackInformation() {}

		/// @brief ダメージ値を取得
		/// @return ダメージ値
		float GetDamageValue() const;
	};
}



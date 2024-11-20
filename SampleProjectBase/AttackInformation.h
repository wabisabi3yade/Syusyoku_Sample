#pragma once

namespace HashiTaku
{
	/// @brief  攻撃の情報
	class AttackInformation : public HashiTaku::ISaveLoad, public HashiTaku::IImGuiUser
	{
	public:
		/// @brief 攻撃レベル
		enum class AttackLevel
		{
			Low, // 低
			Mid,	// 中
			High,	// 高
			SuperHigh	// 最大
		};

	private:
		/// @brief 攻撃時のダメージ
		float atkDamage;

		/// @brief ヒットストップで止めるフレーム数
		u_int hitStopFrame;

		/// @brief 攻撃レベル
		AttackLevel atkLevel;
	public:
		AttackInformation();
		AttackInformation(float _atkDamage, AttackLevel _atkLevel);
		~AttackInformation() {}

		/// @brief ダメージ値をセット
		/// @param _atkDamage ダメージ値
		void SetDamageValue(float _atkDamage);

		/// @brief ヒットストップで止めるフレーム数を取得
		/// @return ヒットストップ数
		void SetHitStopFlame(u_int _hitStopFlame);

		/// @brief 攻撃レベルをセット
		/// @param _atkLevel 攻撃レベル
		void SetAttackLevel(AttackLevel _atkLevel);

		/// @brief ダメージ値を取得
		/// @return ダメージ値
		float GetDamageValue() const;

		/// @brief ヒットストップで止めるフレーム数を取得
		/// @return ヒットストップ数
		u_int GetHitStopFlame() const;

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		/// @brief 攻撃レベルから各パラメータを適用
		void ApplyFromAttackLevel();

		void ImGuiDebug() override;
	private:
		// Low
		static constexpr u_int LOW_HITSTOP = 0;

		// Mid
		static constexpr u_int MID_HITSTOP = 2;

		// High
		static constexpr u_int HIGH_HITSTOP = 4;

		// SuperHigh
		static constexpr u_int SUPERHIGH_HITSTOP = 6;
	};
}



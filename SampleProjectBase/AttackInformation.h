#pragma once
#include "CameraShakeParameter.h"

namespace HashiTaku
{
	class VisualEffect;

	/// @brief  攻撃の情報
	class AttackInformation : public ISaveLoad, public IImGuiUser
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
		/// @brief ヒットエフェクト情報
		CreateVfxInfo hitVfxInfo;

		/// @brief カメラを揺らすパラメータ
		CameraShakeParameter pCamShakeParam;

		/// @brief 攻撃時のダメージ
		float atkDamage;

		/// @brief ヒットストップで止めるフレーム数
		u_int hitStopFrame;

		/// @brief 攻撃レベル
		AttackLevel atkLevel;

		/// @brief カメラを揺らすか？
		bool isCamShake;
	public:
		AttackInformation();
		virtual ~AttackInformation() {}

		/// @brief ダメージ値をセット
		/// @param _atkDamage ダメージ値
		void SetDamageValue(float _atkDamage);

		/// @brief ヒットストップで止めるフレーム数を取得
		/// @return ヒットストップ数
		void SetHitStopFlame(u_int _hitStopFlame);

		/// @brief 攻撃レベルをセット
		/// @param _atkLevel 攻撃レベル
		void SetAttackLevel(AttackLevel _atkLevel);

		/// @brief ヒットエフェクト情報を取得する
		/// @return ヒットエフェクト情報
		const CreateVfxInfo& GetHitVfxInfo() const;

		/// @brief カメラを揺らすパラメータを取得
		/// @return カメラを揺らすパラメータ
		const CameraShakeParameter& GetCamShakeParam() const;

		/// @brief ダメージ値を取得
		/// @return ダメージ値
		float GetDamageValue() const;

		/// @brief ヒットストップで止めるフレーム数を取得
		/// @return ヒットストップ数
		u_int GetHitStopFlame() const;

		/// @brief 攻撃レベルを取得
		/// @return 攻撃レベル
		AttackLevel GetAttackLevel() const;

		/// @brief カメラを揺らすかどうか
		/// @return カメラ揺らす？
		bool GetIsShake() const;

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	protected:
		void ImGuiDebug() override;
	private:
		/// @brief 攻撃レベルから各パラメータを適用
		void ApplyFromAttackLevel();
	private:
		// Low
		static constexpr u_int LOW_HITSTOP = 0;

		// Mid
		static constexpr u_int MID_HITSTOP = 3;

		// High
		static constexpr u_int HIGH_HITSTOP = 5;

		// SuperHigh
		static constexpr u_int SUPERHIGH_HITSTOP = 8;
	};
}



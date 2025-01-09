#pragma once
#include "PerlinShakeParameter.h"

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
			SuperHigh,	// 最大
			MaxNum
		};

	private:
		/// @brief ヒットエフェクト情報
		CreateVfxInfo hitVfxInfo;

		/// @brief カメラを揺らすパラメータ
		PerlinShakeParameter pCamShakeParam;

		/// @brief ヒット音のパラメータ
		PlaySoundParameter hitSoundParameter;

		/// @brief 攻撃時のダメージ
		float atkDamage;

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

		/// @brief 攻撃レベルをセット
		/// @param _atkLevel 攻撃レベル
		void SetAttackLevel(AttackLevel _atkLevel);

		/// @brief ヒットエフェクト情報を取得する
		/// @return ヒットエフェクト情報
		const CreateVfxInfo& GetHitVfxInfo() const;

		/// @brief カメラを揺らすパラメータを取得
		/// @return カメラを揺らすパラメータ
		const PerlinShakeParameter& GetCamShakeParam() const;

		/// @brief ヒットSEを取得
		/// @return ヒットSE
		const PlaySoundParameter& GetHitSEParam() const;

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
		bool GetIsCamShake() const;

		/// @brief パッド振動力を取得
		/// @return パッド振動の力
		float GetPadShakePower() const;

		/// @brief パッド振動の時間取得
		/// @return パッド振動時間
		float GetPadShakeTime() const;

		json Save() override;
		void Load(const json& _data) override;
	protected:

		void ImGuiDebug() override;
	private:

		// 攻撃レベルで変更するパラメータ
		void ImGuiLevelParamerter();
	private:
		static std::array<u_int, static_cast<u_int>(AttackLevel::MaxNum)> hitStopFrames;

		static std::array<float, static_cast<u_int>(AttackLevel::MaxNum)> padShakePowers;

		static std::array<float, static_cast<u_int>(AttackLevel::MaxNum)> padShakeTimes;
	};
}



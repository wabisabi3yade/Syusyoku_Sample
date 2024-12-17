#pragma once
#include "PlayerAttackState.h"

namespace HashiTaku
{
	/// @brief 地上溜め攻撃
	class PlayerGroundChargeAttack : public PlayerAttackState
	{
		/// @brief チャージのレベル
		enum class ChargeLevel
		{
			Low,	// 低
			Mid,	// 中
			High,	// 高
			MaxNum
		};

		/// @brief チャージ段階ごとの攻撃情報
		using ChargeLevelAtkInfos = 
			std::array<PlayerAttackInformation, static_cast<u_int>(ChargeLevel::MaxNum)>;

		/// @brief チャージレベルごとの攻撃情報(各攻撃の)
		std::vector<ChargeLevelAtkInfos> chargeAtkInfos;

		/// @brief レベルごとのチャージの時間
		std::array<float, static_cast<u_int>(ChargeLevel::MaxNum)> chargeTimes;

		/// @brief 最大溜め時間
		float maxChargeTime;

		/// @brief 現在の溜め時間
		float curChargingTime;

		/// @brief 現在のチャージのレベル
		ChargeLevel curChargeLevel;

		/// @brief チャージ中かどうか？
		bool isCharging;
	public:
		PlayerGroundChargeAttack();
		~PlayerGroundChargeAttack() {}

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		/// @brief 開始
		void OnStartBehavior() override;

		/// @brief 更新
		void UpdateBehavior() override;

		/// @brief 攻撃回数を変えたときの処理
		void OnChangeAttackTimes() override;

		/// @brief 入力を離したか確認する
		/// @return 入力離したか？
		void CheckInputRelease();

		/// @brief チャージ中の更新処理
		void ChargingUpdate();

		/// @brief チャージ終了時の処理
		void OnChargeEnd();
	
		void ImGuiDebug() override;

		// 攻撃情報の編集
		void ImGuiAttackInfo() override;
	};
}


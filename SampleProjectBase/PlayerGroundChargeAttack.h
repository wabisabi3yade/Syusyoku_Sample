#pragma once
#include "PlayerAttackState.h"

namespace HashiTaku
{
	class CP_CameraMove;

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

		/// @brief レベルごとのチャージの色
		std::array<DXSimp::Color, static_cast<u_int>(ChargeLevel::MaxNum)> chargeVfxColor;

		/// @brief チャージで次の段階に移行した時のエフェクト
		CreateVfxInfo onNextChargeVfx;

		/// @brief チャージ攻撃に移行するエフェクト
		CreateVfxInfo chargeReleaseVfx;

		/// @brief チャージエフェクトのオフセット座標
		DXSimp::Vector3 chargeVfxOffset;

		/// @brief チャージ攻撃移行時のエフェクトオフセットY座標
		float chargeReleaseVfxOffsetY;

		// カメラ移動コンポーネント
		CP_CameraMove* pCamMove;

		/// @brief 最大溜め時間
		float maxChargeTime;

		/// @brief 現在の溜め時間
		float curChargingTime;

		/// @brief 1フレーム前のの溜め時間
		float lastChargingTime;

		/// @brief チャージ中のエフェクトの生成時間オフセット(チャージ時から見た)
		float chargeVfxCreateTimeOffset;

		/// @brief チャージ中のパッド振動力
		float chargePadShakePower;

		/// @brief 現在のチャージのレベル
		ChargeLevel curChargeLevel;

		/// @brief チャージのエフェクトハンドル
		Effekseer::Handle chargeVfxHandle;

		/// @brief チャージ中かどうか？
		bool isCharging;
	public:
		PlayerGroundChargeAttack();
		~PlayerGroundChargeAttack() {}

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief 開始
		void OnStartBehavior() override;

		/// @brief 更新
		void UpdateBehavior() override;

		/// @brief 終了
		void OnEndBehavior() override;

		/// @brief カメラをチャージ状態に編子うする
		void ChangeCameraChargeState();

		/// @brief 攻撃回数を変えたときの処理
		void OnChangeAttackTimes() override;

		/// @brief 入力を離したか確認する
		/// @return 入力離したか？
		void CheckInputRelease();

		/// @brief チャージ中の更新処理
		void ChargingUpdate();

		/// @brief チャージのエフェクトを出す
		void CreateChargeVfx();

		/// @brief 次のチャージレベルへ移行
		void NextChargeLevel();

		/// @brief チャージ終了時の処理
		void OnChargeEnd();
	
		void ImGuiDebug() override;

		// 攻撃情報の編集
		void ImGuiAttackInfo() override;
	};
}


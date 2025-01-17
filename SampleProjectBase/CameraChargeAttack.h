#pragma once
#include "CameraFollowMove.h"
#include "CalcPerlinShakeVector.h"

namespace HashiTaku
{
	/// @brief プレイヤーのチャージ攻撃時のカメラステート
	class CameraChargeAttack : public CameraFollowMove
	{
		/// @brief チャージ中のシェイクを計算する
		CalcPerlinShakeVector calcChargeingShake;

		/// @brief チャージ中のシェイク状態
		PerlinShakeParameter chargeingShakeParam;

		/// @brief チャージ時のターゲットのFov
		float chargeTargetFov;

		/// @brief 視野角の移動時間
		float moveTargetFovTime;

		/// @brief 視野角移動経過時間
		float elapsedMoveFovTime;

		/// @brief 視野角移動開始時の視野角
		float fovOnBeginMove;

		/// @brief 視野角の移動イージング
		EaseKind moveFovEase;
	public:
		CameraChargeAttack();
		~CameraChargeAttack() {}

		/// @brief 視野角移動を開始する
		/// @param _targetFov ターゲットの視野角
		/// @param _moveTime 移動時間
		void BeginMoveFov(float _targetFov, float _moveTime);

		/// @brief チャージ中の振動を止める
		void EndChargingShake();

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief 開始処理
		void OnStartBehavior() override;

		/// @brief 更新処理
		void UpdateBehavior() override;

		/// @brief 終了
		void OnEndBehavior() override;

		/// @brief 視野角移動更新
		void UpdateFovMove();

		/// @brief チャージ中のシェイク更新
		void ChargeShakeUpdate();

		void ImGuiDebug() override;
	};

}


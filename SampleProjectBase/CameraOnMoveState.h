#pragma once
#include "CameraMoveState_Base.h"

namespace HashiTaku
{
	/// @brief プレイヤー移動時のカメラの行動
	class CameraOnMoveState : public CameraMoveState_Base
	{
		/// @brief カメラポジション
		DirectX::SimpleMath::Vector3 cameraPos;

		/// @brief 追従のワールド座標
		DirectX::SimpleMath::Vector3 followPos;

		/// @brief 入力数値
		DirectX::SimpleMath::Vector2 inputVal;

		/// @brief スティックを倒したときの遊びの大きさ
		DirectX::SimpleMath::Vector2 inpDeadZone;

		/// @brief 何も入力なしのときの高さ
		float idleHeight;

		/// @brief 現在のy距離
		float currentHeight;

		/// @brief 対象のオブジェクトとの最大y距離
		float cameraHeightMax;

		/// @brief 対象のオブジェクトとの最小y距離
		float cameraHeightMin;

		/// @brief 縦方向の速度
		float verticalSpeed;

		/// @brief 待機の高さに戻るときの速度割合
		float returnVertRatio;

		/// @brief 回転速度
		float rotateSpeed;

		/// @brief 横軸の目標への移動速度
		float horiSpeedToTarget;

		/// @brief 中心との角度
		float centerAngle;

		/// @brief 横軸の対象とのカメラ距離
		float distanceHorizon;

		/// @brief ターゲットからY座標の注視点
		float lookTargetOffsetY;

		/// @brief ターゲット処理するか？
		bool isTargeting;
	public:
		CameraOnMoveState();
		~CameraOnMoveState() {}

		/// @brief 初期座標を渡す
		void InitCameraTransform();

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		/// @brief 開始処理
		void OnStartBehavior() override;

		/// @brief 更新処理
		void UpdateBehavior() override;

		void NormalUpdate();

		void TargetUpdate();

		/// @brief 終了処理
		void OnEndBehavior() override;

		/// @brief 遷移確認
		void CheckTransitionUpdate() override;

		/// @brief 入力更新
		void InputUpdate();

		/// @brief 縦軸の移動
		void VerticalMove();

		/// @brief 移動する
		void RotationMove();

		/// @brief 注視点を更新
		void LookUpdate();

		void ImGuiDebug() override;
	};
}
#pragma once
#include "CameraMoveState_Base.h"

namespace HashiTaku
{
	/// @brief プレイヤー移動時のカメラの行動
	class CameraFollowMove : public CameraMoveState_Base
	{
		/// @brief カメラポジション
		DXSimp::Vector3 cameraPos;

		/// @brief 追従のワールド座標
		DXSimp::Vector3 followPos;

		/// @brief 入力数値
		DXSimp::Vector2 inputVal;

		/// @brief スティックを倒したときの遊びの大きさ
		DXSimp::Vector2 inpDeadZone;

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

		/// @brief 中心との角度(状態を変えても保持する為にstatic)
		static float centerAngle;

		/// @brief 横軸の対象とのカメラ距離
		float distanceHorizon;

		/// @brief ターゲットからY座標の注視点
		float lookTargetOffsetY;

		/// @brief 視点移動速度のレート
		float lookSpeedRate;

		/// @brief 視点移動を滑らかに移動している途中か？
		bool isFollowLooking;

		/// @brief ターゲット処理するか？
		bool isTargeting;

		/// @brief ターゲット移動時のfov
		float moveFov;

		/// @brief 通常時Fov
		float normalFov;

		/// @brief fovの変動率
		float fovChangeRate;

		/// @brief fov変動する追従先の移動量
		float canFovChangeMovement;
	public:
		CameraFollowMove();
		virtual ~CameraFollowMove() {}

		/// @brief 初期座標を渡す
		void InitCameraTransform();

		json Save() override;
		void Load(const json& _data) override;

	protected:
		/// @brief 開始処理
		void OnStartBehavior() override;

		/// @brief 更新処理
		void UpdateBehavior() override;

		/// @brief 終了処理
		void OnEndBehavior() override;

		void ImGuiDebug() override;
	private:
		/// @brief 通常時の更新処理
		void NormalUpdate();

		/// @brief ターゲット時の更新処理
		void TargetUpdate();

		/// @brief 移動での視野角移動
		void FovUpdate();

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
	};
}
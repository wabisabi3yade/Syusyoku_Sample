#pragma once
#include "CameraMoveState_Base.h"
#include "GameInput.h"

/// @brief プレイヤー移動時のカメラの行動
class CameraOnMoveState : public CameraMoveState_Base
{
	/// @brief カメラポジション
	DirectX::SimpleMath::Vector3 cameraPos;

	/// @brief ターゲットのワールド座標
	DirectX::SimpleMath::Vector3 targetPos;

	/// @brief 入力数値
	DirectX::SimpleMath::Vector2 inputVal;

	/// @brief ゲーム内入力
	GameInput* pInput;

	/// @brief 現在のy距離
	float currentHeight;

	/// @brief 対象のオブジェクトとの最大y距離
	float cameraHeightMax;

	/// @brief 対象のオブジェクトとの最小y距離
	float cameraHeightMin;

	/// @brief 縦方向の速度
	float verticalSpeed;

	/// @brief 回転速度
	float rotateSpeed;

	/// @brief 中心との角度
	float centerAngle;

	/// @brief 横軸の対象とのカメラ距離
	float distanceHorizon;

	/// @brief ターゲットからY座標の注視点
	float lookTargetOffsetY;

public :
	CameraOnMoveState();
	~CameraOnMoveState() {}

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 開始処理
	void OnStartBehavior();

	/// @brief 更新処理
	void UpdateBehavior();

	/// @brief 終了処理
	void OnEndBehavior();

	/// @brief 縦軸の移動
	void VerticalMove();

	/// @brief 移動する
	void RotationMove();

	/// @brief 注視点を更新
	void LookUpdate();
	
	void ImGuiDebug() override;
};


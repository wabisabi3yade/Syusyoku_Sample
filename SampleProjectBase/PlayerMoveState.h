#pragma once
#include "PlayerActState_Base.h"

/// @brief プレイヤー移動処理
class PlayerMoveState : public PlayerActState_Base
{
	/// @brief 移動方向
	DirectX::SimpleMath::Vector3 moveVector;

	/// @brief 現在の速度
	float currentSpeed;

	/// @brief 最大速度
	float maxSpeed;

	/// @brief 加速度
	float acceleration;

	/// @brief 回転速度
	float rotateSpeed;
public:
	PlayerMoveState();
	~PlayerMoveState() {}

private:
	void OnStart() override;
	void Update() override;
	void OnEnd() override;

	/// @brief 移動
	void Move();

	/// @brief 回転させる
	void Rotation();

	/// @brief	入力値を返す
	DirectX::SimpleMath::Vector2 InputValue();

	/// @brief 移動入力されているか
	bool IsMoveInput();

	void ImGuiSetting() override;
};


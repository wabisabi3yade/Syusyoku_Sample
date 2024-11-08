#pragma once
#include "BossGroundState.h"

/// @brief ボス地上移動
class BossGroundMove : public BossGroundState
{
	/// @brief  最大移動速度
	float maxSpeed;

	/// @brief 加速度
	float acceralation;

	/// @brief 現在速度
	float currentSpeed;
public:
	BossGroundMove();
	~BossGroundMove() {}

private:
	/// @brief 更新処理
	void UpdateBehavior() override;

	/// @brief 移動処理
	void Move();
};


#pragma once
#include"PlayerActState_Base.h"

/// @brief プレイヤージャンプステート
class PlayerJumpState : public PlayerActState_Base
{
	/// @brief ジャンプの力
	float jumpPower;

	/// @brief ジャンプ開始までのかがむ時間
	float crouchTime;

	/// @brief ステート開始からの経過時間
	float stateElapsedTime;

	/// @brief  ジャンプしたかどうか？
	bool isAlreadyJump;
public:
	PlayerJumpState();
	~PlayerJumpState();
	
private:
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;

	/// @brief ジャンプ開始するときの処理
	void OnBeginJump();
};


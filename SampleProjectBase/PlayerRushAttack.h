#pragma once
#include "PlayerGroundAttack.h"

/// @brief 前突進攻撃
class PlayerRushAttack : public PlayerGroundAttack
{
	std::unique_ptr<AnimationCurve> pSpeedCurve;

	/// @brief 移動速度
	float moveSpeed;

	/// @brief 移動時間
	float moveTime;

	/// @brief 移動経過時間
	float elapsedMoveTime;
public:
	PlayerRushAttack();
	~PlayerRushAttack() {}

private:
	// State共通処理
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void TransitionCheckUpdate();

	void ImGuiSetting() override;
};


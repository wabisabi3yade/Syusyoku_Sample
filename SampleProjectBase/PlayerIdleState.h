#pragma once
#include "PlayerActState_Base.h"
class PlayerIdleState : public PlayerActState_Base
{
	/// @brief 放置経過時間
	float leaveElapsedTime;

	/// @brief 特殊待機モーションに移行するまでの時間
	float specialIdleTransTiime;
public:
	PlayerIdleState();
	~PlayerIdleState() {}

private:
	/// @brief 各状態の開始処理
	void OnStartBehavior() override;
	/// @brief 更新処理
	void UpdateBehavior() override;
	/// @brief 各状態の終了処理
	void OnEndBehavior() override;

	/// @brief 放置経過時間を進める
	void ProgressLeaveElapse();

	/// @brief 移動Stateに遷移できるか確認
	/// @return 遷移できるか？
	bool IsCanMoveTransition();
};


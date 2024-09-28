#pragma once
#include "PlayerActState_Base.h"

/// @brief プレイヤー攻撃のゲーム内処理
class PlayerAttackState : public PlayerActState_Base
{
public:
	PlayerAttackState();
	~PlayerAttackState() {}

private:
	void OnStart() override;
	void Update() override;
	void OnEnd() override;

	void ImGuiSetting() override;
};
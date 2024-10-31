#pragma once
#include "PlayerActState_Base.h"
#include "AttackInformation.h"

/// @brief プレイヤー攻撃のゲーム内処理
class PlayerAttackState : public PlayerActState_Base
{
private:
	/// @brief コンビネーション攻撃の先
	StateType nextCombAtkState;

	/// @brief 攻撃情報
	std::unique_ptr<HashiTaku::AttackInformation> pAttackInfo;
public:
	PlayerAttackState();
	virtual ~PlayerAttackState() {}

private:
	// State共通処理
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;
	void OnTransitionCheck();

	/// @brief 攻撃情報を更新する
	void UpdateAttackInfo();

	void ImGuiSetting() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
};
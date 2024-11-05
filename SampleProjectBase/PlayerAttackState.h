#pragma once
#include "PlayerActState_Base.h"
#include "AttackInformation.h"

/// @brief プレイヤー攻撃のゲーム内処理
class PlayerAttackState : public PlayerActState_Base
{
private:
	/// @brief コンビネーション攻撃の先
	PlayerState nextCombAtkState;

	/// @brief 攻撃情報
	std::unique_ptr<HashiTaku::AttackInformation> pAttackInfo;

	/// @brief 先行入力時間
	float senkoInputTime;
public:
	PlayerAttackState();
	virtual ~PlayerAttackState() {}

protected:
	// State共通処理
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;
	void TransitionCheckUpdate();

	void ImGuiSetting() override;
private:
	/// @brief 攻撃情報を更新する
	void UpdateAttackInfo();

	/// @brief キャンセルできるか確認する
	/// @return キャンセルできるか？
	bool CheckCanCancel() const;
	
	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

public:
	constexpr static auto ATTACKTRIGGER_PARAMNAME = "attackTrigger";	// 攻撃トリガー
};
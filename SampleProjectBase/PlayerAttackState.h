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

	/// @brief 前に進む速度カーブ
	AnimationCurve forwardSpeedCurve;

	/// @brief 進む距離
	float atkMoveSpeed;

	/// @brief 先行入力時間
	float senkoInputTime;

	/// @brief 前へ進むか？
	bool isMoveForward;
public:
	PlayerAttackState();
	virtual ~PlayerAttackState() {}

protected:
	// State共通処理
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;
	void TransitionCheckUpdate() override;

	void ImGuiDebug() override;
private:
	/// @brief 攻撃情報を更新する
	void UpdateAttackInfo();

	/// @brief 攻撃時に前へ進む
	void ForwardProgressMove();
	
	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

public:
	constexpr static auto ATTACKTRIGGER_PARAMNAME = "attackTrigger";	// 攻撃トリガー
};
#pragma once
#include "BossAttackState.h"

/// @brief ボスのコンビネーション攻撃をセット
class BossCombAttack : public BossAttackState
{
	/// @brief リアタックをするトリガー
	const bool* pIsReAttack;

	/// @brief コンビネーション攻撃の攻撃回数
	u_int attackTimeCnt;

	/// @brief 現在、攻撃の何段目か
	u_int curAttackTime;
public:
	BossCombAttack();
	virtual ~BossCombAttack() {}

	/// @brief 開始処理
	void OnStartBehavior() override;

	/// @brief 更新処理
	void UpdateBehavior() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief コンビネーション攻撃の攻撃に合わせて攻撃情報を更新する
	void UpdateReAttack();

	/// @brief 攻撃回数をセット
	/// @param _attackTime 攻撃回数
	void SetAttackTimeCnt(u_int _attackTime);

	void ImGuiDebug();
};


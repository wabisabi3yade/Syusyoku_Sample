#pragma once
#include "BossDamageState.h"

/// @brief ボスブレイク終了のノック
class BossBreakEndKnock : public BossDamageState
{
	/// @brief ブレイク終了できるブレイク値
	float canBreakValue;

	/// @brief ノックの距離
	float knockDistance;
public:
	BossBreakEndKnock();
	~BossBreakEndKnock() {}

	/// @brief ブレイク値を取得する
	float GetCanBreakValue() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 開始
	void OnStartBehavior() override;

	/// @brief ノックするターゲットの座標を求める
	void CalcKnockTargetPos();

	/// @brief アニメーション終了時の行動
	/// @param _fromAnimNodeName 遷移元のアニメーションノード名
	/// @param _toAnimNodeName 遷移先のアニメーションノード名
	void OnAnimationEnd(const std::string& _fromAnimNodeName,
		const std::string& _toAnimNodeName) override;

	void ImGuiDebug() override;
};


#pragma once
#include "BossGroundState.h"

/// @brief 地上での攻撃
class BossGroundAttack : public BossGroundState
{

public:
	BossGroundAttack();
	virtual ~BossGroundAttack() {}

	/// @brief 開始
	void OnStartBehavior() override;

	/// @brief アニメーション終了時の行動
	/// @param _fromAnimNodeName 遷移元のアニメーションノード名
	/// @param _toAnimNodeName 遷移先のアニメーションノード名
	void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;
};


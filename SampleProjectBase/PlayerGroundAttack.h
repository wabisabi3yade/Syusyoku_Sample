#pragma once
#include "PlayerAttackState.h"

// プレイヤー地上攻撃状態
class PlayerGroundAttack : public PlayerAttackState
{
public:
	PlayerGroundAttack() {}
	~PlayerGroundAttack() {}

	void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;
};


#pragma once
#include "PlayerAttackState.h"

// �v���C���[�n��U�����
class PlayerGroundAttack : public PlayerAttackState
{
public:
	PlayerGroundAttack() {}
	~PlayerGroundAttack() {}

	void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;
};


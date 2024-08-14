#pragma once
#include "PlayerActState_Base.h"

/// @brief �v���C���[�U���̃Q�[��������
class PlayerAttackState : public PlayerActState_Base
{
public:
	PlayerAttackState(PlayerActionController& _pController);
	~PlayerAttackState() {}

	void Init() override;

	void Terminal() override;

private:
	void Update() override;

	void TransitionCheck() override;
};


#pragma once
#include "PlayerActState_Base.h"

/// @brief �v���C���[�U���̃Q�[��������
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
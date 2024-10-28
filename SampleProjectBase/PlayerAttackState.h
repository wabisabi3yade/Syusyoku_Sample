#pragma once
#include "PlayerActState_Base.h"

/// @brief �v���C���[�U���̃Q�[��������
class PlayerAttackState : public PlayerActState_Base
{
private:


public:
	PlayerAttackState();
	~PlayerAttackState() {}

private:
	void OnStart() override;
	void Update() override;
	void OnEnd() override;

	void ImGuiSetting() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
};
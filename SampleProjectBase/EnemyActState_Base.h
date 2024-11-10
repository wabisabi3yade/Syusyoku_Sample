#pragma once
#include "CharacterActState_Base.h"

/// @brief �G�̃A�N�V�����X�e�[�g���
class EnemyActState_Base : public CharacterActState_Base
{

public:
	EnemyActState_Base();
	virtual ~EnemyActState_Base() {}

	virtual nlohmann::json Save() = 0;
	virtual void Load(const nlohmann::json& _data) = 0;

protected:
	virtual void ImGuiDebug() = 0;
};


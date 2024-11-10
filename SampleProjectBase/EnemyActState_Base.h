#pragma once
#include "CharacterActState_Base.h"

/// @brief 敵のアクションステート基底
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


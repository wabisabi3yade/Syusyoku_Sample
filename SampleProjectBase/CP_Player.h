#pragma once
#include "Component.h"

#include "PlayerAnimController.h"
#include "PlayerActionController.h"

class CP_Player : public Component
{
	std::unique_ptr<PlayerAnimController> pAnimController;

	/// @brief アクションコントローラー
	std::unique_ptr<PlayerActionController> pActionController;

public:
	CP_Player() {}
	~CP_Player() {}

	void Init() override;

	void Update() override;

	void ImGuiSetting() override;
};


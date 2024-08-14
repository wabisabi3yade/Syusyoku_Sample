#pragma once
#include "Component.h"

#include "PlayerAnimController.h"
#include "PlayerActionController.h"
#include "PlayerAnimObserver.h"

class CP_Player : public Component
{
	std::unique_ptr<PlayerAnimController> pAnimController;

	/// @brief �A�N�V�����R���g���[���[
	std::unique_ptr<PlayerActionController> pActionController;

	/// @brief �A�j���[�V�����I�u�U�[�o�[
	std::unique_ptr<PlayerAnimObserver> pAnimObserver;
public:
	CP_Player() {}
	~CP_Player() {}

	void Init() override;

	void Update() override;

	void ImGuiSetting() override;
};


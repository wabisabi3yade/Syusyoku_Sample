#pragma once
#include "Component.h"

#include "PlayerAnimController.h"

class CP_Player : public Component
{
	std::unique_ptr<PlayerAnimController> pAnimController;

public:
	CP_Player() {}
	~CP_Player() {}

	void Init() override;
};


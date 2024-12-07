#pragma once
#include "PlayerGroundState.h"
class PlayerIdleState : public PlayerGroundState
{
public:
	PlayerIdleState();
	~PlayerIdleState() {}

private:
	void OnStartBehavior() override;
};


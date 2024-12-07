#include "pch.h"
#include "PlayerIdleState.h"
#include "CP_Player.h"

PlayerIdleState::PlayerIdleState()
{
}

void PlayerIdleState::OnStartBehavior()
{
	pActionController->SetAnimationFloat(SPEEDRATIO_PARAMNAME, 0.0f);
}

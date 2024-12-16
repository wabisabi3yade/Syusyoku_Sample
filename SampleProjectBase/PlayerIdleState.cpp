#include "pch.h"
#include "PlayerIdleState.h"
#include "CP_Player.h"

namespace HashiTaku
{
	PlayerIdleState::PlayerIdleState()
	{
		SetTargetAtEnemy(true);
	}

	void PlayerIdleState::OnStartBehavior()
	{
		pActionController->SetAnimationFloat(SPEEDRATIO_PARAMNAME, 0.0f);
	}
}
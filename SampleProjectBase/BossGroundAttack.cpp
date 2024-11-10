#include "pch.h"
#include "BossGroundAttack.h"
#include "BossActionController.h"
#include "CP_Animation.h"

BossGroundAttack::BossGroundAttack()
{
}

void BossGroundAttack::OnStartBehavior()
{
	BossGroundState::OnStartBehavior();

	// UŒ‚ƒgƒŠƒK[
	pActionController->GetAnimation().SetTrigger(ATTACKTRIGGER_ANIMPARAM_NAME);
}

void BossGroundAttack::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	if (_toAnimNodeName == RUN_ANIM_NAME)
	{
		ChangeState(BossState::Run);
	}
}

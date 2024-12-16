#include "pch.h"
#include "BossBreakIdle.h"
#include "CP_Boss.h"

namespace HashiTaku
{
	void BossBreakIdle::TransitionCheckUpdate()
	{
		if (!pActionController->GetIsBreaking())
			ChangeState(BossState::Idle);
	}
}
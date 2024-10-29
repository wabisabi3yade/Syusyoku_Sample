#include "pch.h"
#include "PlayerGroundAttack.h"

void PlayerGroundAttack::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	if (_toAnimNodeName == "Idle")
		ChangeState(StateType::Idle);
}

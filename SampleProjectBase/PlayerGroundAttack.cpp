#include "pch.h"
#include "PlayerGroundAttack.h"

void PlayerGroundAttack::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	// アニメーションの遷移先が待機状態なら待機に戻す
	if (_toAnimNodeName == IDLE_ANIM_NAME)
		ChangeState(PlayerState::Idle);
}

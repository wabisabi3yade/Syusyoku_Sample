#include "pch.h"
#include "PlayerGroundAttack.h"

void PlayerGroundAttack::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	// �A�j���[�V�����̑J�ڐ悪�ҋ@��ԂȂ�ҋ@�ɖ߂�
	if (_toAnimNodeName == IDLE_ANIM_NAME)
		ChangeState(PlayerState::Idle);
}

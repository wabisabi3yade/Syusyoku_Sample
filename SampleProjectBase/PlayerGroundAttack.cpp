#include "pch.h"
#include "PlayerGroundAttack.h"

constexpr auto IDLE_ANIMNODE_NAME("Idle");	// �ҋ@��Ԃ̃A�j���[�V�����m�[�h��
constexpr auto TARGETIDLE_ANIMNODE_NAME("TargetIdle");	// �^�[�Q�b�g�ҋ@��Ԃ̃A�j���[�V�����m�[�h��

void PlayerGroundAttack::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	// �A�j���[�V�����̑J�ڐ悪�ҋ@��ԂȂ�ҋ@�ɖ߂�
	if (_toAnimNodeName == IDLE_ANIMNODE_NAME || _toAnimNodeName == TARGETIDLE_ANIMNODE_NAME)
		ChangeState(PlayerState::Idle);
}

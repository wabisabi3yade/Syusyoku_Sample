#include "pch.h"
#include "AnimNodePlayer_Base.h"
#include "SkeletalMesh.h"
#include "AnimationNode_Base.h"

AnimNodePlayer_Base::AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList)
	: pPlayAnimNode(&_playNode), pBoneList(&_boneList), curPlayRatio(0.0f), lastPlayRatio(-Mathf::smallValue)
{
	// �m�[�h�݂̂̍Đ����x�����߂�
	progressNodeSpeed = 1.0f / pPlayAnimNode->GetAnimationTime();
}

void AnimNodePlayer_Base::UpdateCall(float _controllerPlaySpeed)
{
	// �Đ�������i�߂�
	ProgressPlayRatio(_controllerPlaySpeed);

	Update();
}

void AnimNodePlayer_Base::SetCurPlayRatio(float _playRatio)
{
	curPlayRatio = _playRatio;
}
float AnimNodePlayer_Base::GetCurPlayRatio() const
{
	return curPlayRatio;
}

void AnimNodePlayer_Base::ProgressPlayRatio(float _controllerPlaySpeed)
{
	// 1�t���[���O�̍Đ��������X�V
	lastPlayRatio = curPlayRatio;

	// ���Ԃ�i�߂�
	float nodePlayTimeSpeed = pPlayAnimNode->GetPlaySpeedTimes();

	/* 
	�R���g���[���S�̂̍Đ����x �~
	�m�[�h�̍Đ����x�@�~
	�m�[�h�̑��x�{���@�~
	DeletTime
	*/
	curPlayRatio += _controllerPlaySpeed * progressNodeSpeed *  nodePlayTimeSpeed * MainApplication::DeltaTime();

	if (IsCanLoop())	// ���[�v�ł���Ȃ�
		curPlayRatio -= 1.0f;
}

bool AnimNodePlayer_Base::IsCanLoop() const
{
	// �A�j���[�V�����̑S�̎��Ԃ𒴂��Ă��Ȃ��Ȃ�
	if (curPlayRatio < 1.0f) return false;
	if (!pPlayAnimNode->GetIsLoop()) return false;

	return true;
}

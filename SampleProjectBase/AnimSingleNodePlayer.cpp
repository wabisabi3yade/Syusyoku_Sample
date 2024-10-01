#include "pch.h"
#include "AnimSingleNodePlayer.h"

AnimSingleNodePlayer::AnimSingleNodePlayer(const AnimationNode_Base& _singleNode, BoneList& _boneList)
	: AnimNodePlayer_Base(_singleNode, _boneList)
{
}

void AnimSingleNodePlayer::Update()
{
	float playingRatio = GetCurPlayRatio();

	//�{�[�������[�v
	for (unsigned int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		Bone& bone = pBoneList->GetBone(b_i);
		BoneTransform transform;

		// �Đ����Ԃ���e�p�����[�^���擾
		pPlayAnimNode->GetCurAnimTransform(transform, b_i);

		bone.SetAnimTransform(transform);
	}
}

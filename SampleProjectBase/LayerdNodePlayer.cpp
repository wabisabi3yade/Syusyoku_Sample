#include "pch.h"
#include "LayerdNodePlayer.h"

LayerdNodePlayer::LayerdNodePlayer(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform) :
	AnimNodePlayer_Base(_playNode, _boneList, _transform)
{
}

void LayerdNodePlayer::Update(std::vector<BoneTransform>& _outTransforms)
{
	_outTransforms.resize(pAssetBoneList->GetBoneCnt());

	//�{�[�������[�v
	for (unsigned int b_i = 0; b_i < pAssetBoneList->GetBoneCnt(); b_i++)
	{
		// �Đ����Ԃ���e�p�����[�^���擾
		pPlayAnimNode->GetAnimTransform(_outTransforms[b_i], b_i, GetCurPlayRatio());
	}
}

void LayerdNodePlayer::CalcRootMotionPosSpeed()
{

}

void LayerdNodePlayer::ImGuiDebug()
{
	AnimNodePlayer_Base::ImGuiDebug();
}

#include "pch.h"
#include "SingleAnimationNode.h"

// �������
#include "InertInterpAnimation.h"

#include "SkeletalMesh.h"

#include "AssetGetter.h"

using namespace DirectX::SimpleMath;

void SingleAnimationNode::ImGuiPlaying()
{
	AnimationNode_Base::ImGuiPlaying();
}


void SingleAnimationNode::Update(float _playingRatio, BoneList& _boneList)
{
	//�{�[�������[�v
	for (unsigned int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		Bone& bone = _boneList.GetBone(b_i);

		BoneTransform transform;

		// �Đ����Ԃ���e�p�����[�^���擾
		// �X�P�[��
		transform.scale = pAnimationData->GetScaleByRatio(b_i, _playingRatio);

		//�N�H�[�^�j�I��
		transform.rotation = pAnimationData->GetQuaternionByRatio(b_i, _playingRatio);

		// ���W
		transform.position = pAnimationData->GetPositionByRatio(b_i, _playingRatio);

		bone.SetAnimTransform(transform);
	}
}

void SingleAnimationNode::SetAnimationData(const std::string& _animName)
{
	AnimationData* pData = AssetGetter::GetAsset<AnimationData>(_animName);

	if (!pData)	// ����Ɏ擾�ł��܂���ł���
	{
		HASHI_DEBUG_LOG(_animName + ":�A�j���[�V�������擾�ł��܂���ł���");
		return;
	}

	pAnimationData = pData;

	SetAnimationTime(pAnimationData->GetAnimationTime());
}

const AnimationData& SingleAnimationNode::GetAnimationData() const
{
	return *pAnimationData;
}

void SingleAnimationNode::GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const
{
	_outTransform = pAnimationData->GetTransformByRatio(_boneId, _requestRatio);
}

void SingleAnimationNode::ImGuiSetting()
{
	AnimationNode_Base::ImGuiSetting();
}



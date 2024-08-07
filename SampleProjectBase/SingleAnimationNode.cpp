#include "pch.h"
#include "SingleAnimationNode.h"

// �������
#include "InertInterpAnimation.h"

#include "SkeletalMesh.h"

using namespace DirectX::SimpleMath;

void SingleAnimationNode::ImGuiPlaying()
{
	std::string text = "���ԁF" + std::to_string(pAnimationData->GetAnimationTime()) + "(s)";
	ImGui::Text(TO_UTF8(text));
}

void SingleAnimationNode::ImGuiSetting()
{
	AnimationNode_Base::ImGuiSetting();
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
		transform.scale = pAnimationData->GetScale(b_i, _playingRatio);

		//�N�H�[�^�j�I��
		transform.rotation = pAnimationData->GetQuaternion(b_i, _playingRatio);

		// ���W
		transform.position = pAnimationData->GetPosition(b_i, _playingRatio);

		bone.SetAnimTransform(transform);
	}
}

void SingleAnimationNode::SetAnimationData(AnimationData& _animData)
{
	pAnimationData = &_animData;

	SetAnimationTime(pAnimationData->GetAnimationTime());
}

const AnimationData& SingleAnimationNode::GetAnimationData() const
{
	return *pAnimationData;
}
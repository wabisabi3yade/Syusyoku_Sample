#include "pch.h"
#include "AnimSingleNodePlayer.h"
#include "SingleAnimationNode.h"

AnimSingleNodePlayer::AnimSingleNodePlayer(const AnimationNode_Base& _singleNode, BoneList& _boneList, Transform& _transform)
	: AnimNodePlayer_Base(_singleNode, _boneList, _transform)
{
}

void AnimSingleNodePlayer::Update(std::vector<BoneTransform>& _outTransforms)
{
	float playingRatio = GetAnimationRatio();
	_outTransforms.resize(pAssetBoneList->GetBoneCnt());

	//�{�[�������[�v
	for (unsigned int b_i = 0; b_i < pAssetBoneList->GetBoneCnt(); b_i++)
	{
		// �Đ����Ԃ���e�p�����[�^���擾
		pPlayAnimNode->GetAnimTransform(_outTransforms[b_i], b_i, playingRatio);
	}
}

void AnimSingleNodePlayer::CalcRootMotionPosSpeed(float _controllerSpeed)
{
	// �R���g���[���[��
	const SingleAnimationNode& singleNode = static_cast<const SingleAnimationNode&>(*pPlayAnimNode);

	rootMotionPosSpeedPerSec = _controllerSpeed * singleNode.GetRootMotionPosSpeed() * pAssetBoneList->GetLoadScale();
}

DirectX::SimpleMath::Vector3 AnimSingleNodePlayer::GetRootMotionPos(float _ratio, bool _isLoadScaling) const
{
	using namespace DirectX::SimpleMath;

	const SingleAnimationNode& singleNode = static_cast<const SingleAnimationNode&>(*pPlayAnimNode);
	DirectX::SimpleMath::Vector3 rootMotionPos = singleNode.GetAnimationData().GetRootMotionPos(_ratio);

	// ���[�h���̉�]�ʂƁA�X�P�[�����|����
	if (_isLoadScaling)
		ApplyLoadTransform(rootMotionPos);

	return rootMotionPos;
}

DirectX::SimpleMath::Quaternion AnimSingleNodePlayer::GetRootMotionRot(float _ratio, bool _isLoadScaling) const
{
	using namespace DirectX::SimpleMath;
	const SingleAnimationNode& singleNode = static_cast<const SingleAnimationNode&>(*pPlayAnimNode);
	DirectX::SimpleMath::Quaternion rootMotionRot; //= singleNode.GetAnimationData().GetRootMotionRot(_ratio);

	// ���[�h���̉�]�ʂ��|����
	//if (_isLoadScaling)
	//	rootMotionRot = Quat::Multiply(rootMotionRot, pAssetBoneList->GetLoadRotation());

	return rootMotionRot;
}

float AnimSingleNodePlayer::GetModelScale() const
{
	return pAssetBoneList->GetLoadScale();
}

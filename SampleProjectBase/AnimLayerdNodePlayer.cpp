#include "pch.h"
#include "AnimLayerdNodePlayer.h"
#include "LayerdAnimationNode.h"

AnimLayerdNodePlayer::AnimLayerdNodePlayer(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform) :
	AnimNodePlayer_Base(_playNode, _boneList, _transform),
	curBlendPlayRatio(0.0f)
{
	const LayerdAnimationNode& layerNode = static_cast<const LayerdAnimationNode&>(_playNode);

	// �m�[�h�Ō��߂�ꂽ�Đ���������n�߂�
	curBlendPlayRatio = layerNode.GetBeginBlendPlayRatio();
}

void AnimLayerdNodePlayer::ProgressPlayRatio(float _controllerSpeed)
{
	const LayerdAnimationNode& layerdNode = static_cast<const LayerdAnimationNode&>(*pPlayAnimNode);

	AnimNodePlayer_Base::ProgressPlayRatio(_controllerSpeed);

	// 0���Z�h�~

	float speed =
		GetPlayerSpeed() *
		layerdNode.GetBlendCurveSpeed(curBlendPlayRatio) *
		pPlayAnimNode->GetPlaySpeedTimes();

	// �i�߂�
	curBlendPlayRatio += speed * deltaTime / layerdNode.GetBlendAnimationTime();

	//// ���[�v�������Ȃ�����Ή������܂�
	//curBlendPlayRatio = std::min(curBlendPlayRatio, 1.0f);

	if (curBlendPlayRatio > 1.0f)
		curBlendPlayRatio -= 1.0f;
}

void AnimLayerdNodePlayer::Update(std::vector<BoneTransform>& _outTransforms)
{
	_outTransforms.resize(pAssetBoneList->GetBoneCnt());

	//�{�[�������[�v
	const LayerdAnimationNode& layerdNode = static_cast<const LayerdAnimationNode&>(*pPlayAnimNode);

	// �Đ����Ԃ���e�p�����[�^���擾
	layerdNode.GetAnimTransform(_outTransforms, GetCurPlayRatio(), curBlendPlayRatio);

}

void AnimLayerdNodePlayer::CalcRootMotionPosSpeed()
{

}

DirectX::SimpleMath::Vector3 AnimLayerdNodePlayer::GetRootMotionPos(float _ratio, bool _isWorldScaling) const
{
	return DirectX::SimpleMath::Vector3();
}

void AnimLayerdNodePlayer::ImGuiDebug()
{
	AnimNodePlayer_Base::ImGuiDebug();
}

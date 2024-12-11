#include "pch.h"
#include "AnimLayerdNodePlayer.h"
#include "LayerdAnimationNode.h"

AnimLayerdNodePlayer::AnimLayerdNodePlayer(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform) :
	AnimNodePlayer_Base(_playNode, _boneList, _transform),
	curBlendPlayRatio(0.0f)
{
	const LayerdAnimationNode& layerNode = static_cast<const LayerdAnimationNode&>(_playNode);

	// ノードで決められた再生割合から始める
	curBlendPlayRatio = layerNode.GetBeginBlendPlayRatio();
}

void AnimLayerdNodePlayer::ProgressPlayRatio(float _controllerSpeed)
{
	const LayerdAnimationNode& layerdNode = static_cast<const LayerdAnimationNode&>(*pPlayAnimNode);

	AnimNodePlayer_Base::ProgressPlayRatio(_controllerSpeed);

	// 0徐算防止

	float speed =
		GetPlayerSpeed() *
		layerdNode.GetBlendCurveSpeed(curBlendPlayRatio) *
		pPlayAnimNode->GetPlaySpeedTimes();

	// 進める
	curBlendPlayRatio += speed * deltaTime / layerdNode.GetBlendAnimationTime();

	//// ループしたくなったら対応させます
	//curBlendPlayRatio = std::min(curBlendPlayRatio, 1.0f);

	if (curBlendPlayRatio > 1.0f)
		curBlendPlayRatio -= 1.0f;
}

void AnimLayerdNodePlayer::Update(std::vector<BoneTransform>& _outTransforms)
{
	_outTransforms.resize(pAssetBoneList->GetBoneCnt());

	//ボーン数ループ
	const LayerdAnimationNode& layerdNode = static_cast<const LayerdAnimationNode&>(*pPlayAnimNode);

	// 再生時間から各パラメータを取得
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

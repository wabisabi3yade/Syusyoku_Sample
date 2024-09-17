#include "pch.h"
#include "SingleAnimationNode.h"

// 慣性補間
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
	//ボーン数ループ
	for (unsigned int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		Bone& bone = _boneList.GetBone(b_i);

		BoneTransform transform;

		// 再生時間から各パラメータを取得
		// スケール
		transform.scale = pAnimationData->GetScaleByRatio(b_i, _playingRatio);

		//クォータニオン
		transform.rotation = pAnimationData->GetQuaternionByRatio(b_i, _playingRatio);

		// 座標
		transform.position = pAnimationData->GetPositionByRatio(b_i, _playingRatio);

		bone.SetAnimTransform(transform);
	}
}

void SingleAnimationNode::SetAnimationData(const std::string& _animName)
{
	AnimationData* pData = AssetGetter::GetAsset<AnimationData>(_animName);

	if (!pData)	// 正常に取得できませんでした
	{
		HASHI_DEBUG_LOG(_animName + ":アニメーションを取得できませんでした");
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



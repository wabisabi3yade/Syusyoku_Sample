#include "pch.h"
#include "SingleAnimationNode.h"

// 慣性補間
#include "InertInterpAnimation.h"

#include "SkeletalMesh.h"

using namespace DirectX::SimpleMath;

void SingleAnimationNode::ImGuiPlaying()
{
	std::string text = "時間：" + std::to_string(pAnimationData->GetAnimationTime()) + "(s)";
	ImGui::Text(TO_UTF8(text));
}

void SingleAnimationNode::ImGuiSetting()
{
	AnimationNode_Base::ImGuiSetting();
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
		transform.scale = pAnimationData->GetScale(b_i, _playingRatio);

		//クォータニオン
		transform.rotation = pAnimationData->GetQuaternion(b_i, _playingRatio);

		// 座標
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
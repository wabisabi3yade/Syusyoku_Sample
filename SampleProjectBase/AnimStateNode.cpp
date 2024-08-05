#include "pch.h"
#include "AnimStateNode.h"

// 慣性補間
#include "InertInterpAnimation.h"

using namespace DirectX::SimpleMath;

void AnimStateNode::ImGuiSetting()
{
	std::string text = "名前：";
	text += pAnimationData->GetAssetName();
	ImGui::Text(TO_UTF8(text));

	text = "時間：" + std::to_string(pAnimationData->GetAnimationTime()) + "(s)";
	ImGui::Text(TO_UTF8(text));

	ImGui::Checkbox("isLoop", &isLoop);
}

void AnimStateNode::SetAnimationData(AnimationData& _animData)
{
	pAnimationData = &_animData;
}

std::string AnimStateNode::GetNodeName() const
{
	return nodeName;
}

const AnimationData& AnimStateNode::GetAnimationData() const
{
	return *pAnimationData;
}

bool AnimStateNode::GetIsLoop() const
{
	return isLoop;
}
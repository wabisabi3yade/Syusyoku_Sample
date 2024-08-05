#include "pch.h"
#include "AnimStateNode.h"

// �������
#include "InertInterpAnimation.h"

using namespace DirectX::SimpleMath;

void AnimStateNode::ImGuiSetting()
{
	std::string text = "���O�F";
	text += pAnimationData->GetAssetName();
	ImGui::Text(TO_UTF8(text));

	text = "���ԁF" + std::to_string(pAnimationData->GetAnimationTime()) + "(s)";
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
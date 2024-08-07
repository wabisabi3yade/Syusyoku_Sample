#include "pch.h"
#include "AnimationNode_Base.h"

void AnimationNode_Base::ImGuiPlaying()
{
	std::string text = "ñºëOÅF";
	text += nodeName;
	ImGui::Text(TO_UTF8(text));

	ImGui::Checkbox("isLoop", &isLoop);

	ImGui::DragFloat("animationTime", &animationTime, 0.1f, 0.0f, 100.0f);
}

void AnimationNode_Base::ImGuiSetting()
{
	
}

void AnimationNode_Base::SetNodeName(const std::string& _nodeName)
{
	nodeName = _nodeName;
}

std::string AnimationNode_Base::GetNodeName() const
{
	return nodeName;
}

AnimationNode_Base::NodeType AnimationNode_Base::GetNodeType() const
{
	return nodeType;
}

float AnimationNode_Base::GetAnimationTime() const
{
	return animationTime;
}

bool AnimationNode_Base::GetIsLoop() const
{
	return isLoop;
}

void AnimationNode_Base::SetAnimationTime(float _time)
{
	animationTime = std::max(_time, 0.0f);
}

#include "pch.h"
#include "AnimTransitionArrow.h"

#include "AnimationNode_Base.h"

//AnimTransitionArrow::AnimTransitionArrow()
//	: pFromNode(nullptr), pToNode(nullptr)
//{
//}

AnimTransitionArrow::AnimTransitionArrow(AnimationNode_Base& _pFromNode, AnimationNode_Base& _pToNode, float _transTargetRatio, std::function<bool()> _condition)
	: pFromNode(&_pFromNode), pToNode(&_pToNode), condition(_condition), transTargetRatio(_transTargetRatio)
{
}

bool AnimTransitionArrow::CheckTransition()
{
	return condition ? condition() : false;
}

void AnimTransitionArrow::SetCondition(std::function<bool()> _condition)
{
	if (!_condition)
	{
		HASHI_DEBUG_LOG("セットした条件が空です");
		return;
	}

	condition = _condition;
}

AnimationNode_Base& AnimTransitionArrow::GetToNode()
{
	assert(pToNode != nullptr && "遷移先のノードが設定されていません");
	return *pToNode;
}

float AnimTransitionArrow::GetTargetRatio() const
{
	return transTargetRatio;
}

void AnimTransitionArrow::ImGuiSetting()
{
	std::string text = "->" + pToNode->GetNodeName();
	ImGui::Text(text.c_str());
	ImGui::SameLine();
	ImGui::DragFloat("transRatio", &transTargetRatio, 0.1f, 0.0f, 1.0f);
}

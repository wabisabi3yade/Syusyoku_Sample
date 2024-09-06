#include "pch.h"
#include "AnimTransitionArrow.h"

#include "AnimationNode_Base.h"

using namespace HashiTaku;

AnimTransitionArrow::AnimTransitionArrow(AnimationNode_Base& _pFromNode, AnimationNode_Base& _pToNode, float _transTargetRatio, float _transitionTime, std::function<bool()> _condition, EaseKind _easeKind)
	: pFromNode(&_pFromNode), pToNode(&_pToNode), transTargetRatio(_transTargetRatio), transitionTime(_transitionTime), condition(_condition), easeKind(_easeKind), interpolateKind(AnimInterpolateKind::CrossFade)
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

void AnimTransitionArrow::SetEaseKind(HashiTaku::EaseKind _easeKind)
{
	easeKind = _easeKind;
}

void AnimTransitionArrow::SeInterpolateKind(HashiTaku::AnimInterpolateKind _interpolateKind)
{
	interpolateKind = _interpolateKind;
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

float AnimTransitionArrow::GetTransitionTime() const
{
	return transitionTime;
}

HashiTaku::EaseKind AnimTransitionArrow::GetEaseKind() const
{
	return easeKind;
}

HashiTaku::AnimInterpolateKind AnimTransitionArrow::GetInterpolateKind() const
{
	return interpolateKind;
}

void AnimTransitionArrow::ImGuiSetting()
{
	std::string text = "->" + pToNode->GetNodeName();
	ImGui::Text(text.c_str());
	ImGui::SameLine();
	ImGui::DragFloat("transRatio", &transTargetRatio, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("transitionTime", &transitionTime, 0.01f, 0.0f, 10.0f);

	HashiTaku::Easing::ImGuiSelect(easeKind);

	// アニメーション遷移種類
	int selectTransition = static_cast<int>(interpolateKind);
	ImGui::Text("Transition");
	int id = static_cast<int>(AnimInterpolateKind::CrossFade);
	ImGui::RadioButton("CrossFade", &selectTransition, id);
	ImGui::SameLine();
	id = static_cast<int>(AnimInterpolateKind::Inertialization);
	ImGui::RadioButton("Inertialization", &selectTransition, id);
	interpolateKind = static_cast<AnimInterpolateKind>(selectTransition);
}

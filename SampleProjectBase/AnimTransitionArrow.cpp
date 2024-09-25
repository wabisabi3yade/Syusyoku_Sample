#include "pch.h"
#include "AnimTransitionArrow.h"

#include "AnimationNode_Base.h"
#include "TransConditionCreater.h"

using namespace HashiTaku;

AnimTransitionArrow::AnimTransitionArrow(AnimationNode_Base& _fromNode, AnimationNode_Base& _toNode)
	: pFromNode(&_fromNode), pToNode(&_toNode), transTargetRatio(0.0f), transitionTime(0.2f), easeKind(HashiTaku::EaseKind::Linear), interpolateKind(AnimInterpolateKind::CrossFade)
{
}

bool AnimTransitionArrow::CheckTransition()
{
	return false;
}

void AnimTransitionArrow::AddCondition(conditionValType& _val)
{
	// 遷移条件を変数から作成し、追加
	conditionList.push_back(TransConditionCreater::Create(_val));
}

void AnimTransitionArrow::SetTransTargetRatio(float _transTargetRatio)
{
	transTargetRatio = _transTargetRatio;
}

void AnimTransitionArrow::SetTransitonTime(float _transTime)
{
	transitionTime = _transTime;
}

void AnimTransitionArrow::SetEaseKind(HashiTaku::EaseKind _easeKind)
{
	easeKind = _easeKind;
}

void AnimTransitionArrow::SeInterpolateKind(HashiTaku::AnimInterpolateKind _interpolateKind)
{
	interpolateKind = _interpolateKind;
}

AnimationNode_Base& AnimTransitionArrow::GetToNode() const
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

nlohmann::json AnimTransitionArrow::Save()
{
	return nlohmann::json();
}

void AnimTransitionArrow::Load(const nlohmann::json& _data)
{
}

void AnimTransitionArrow::ImGuiSetting()
{
	if (!ImGuiMethod::TreeNode(pToNode->GetNodeName().c_str())) return;
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

	ImGui::TreePop();
}

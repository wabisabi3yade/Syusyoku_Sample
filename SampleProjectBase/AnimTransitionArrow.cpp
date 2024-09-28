#include "pch.h"
#include "AnimTransitionArrow.h"
#include "AnimationNode_Base.h"
#include "TransConditionCreater.h"

using namespace HashiTaku;

AnimTransitionArrow::AnimTransitionArrow(AnimationNode_Base& _fromNode, AnimationNode_Base& _toNode, AnimationParameters& _animParameters)
	: pFromNode(&_fromNode), pToNode(&_toNode), pAnimParameters(&_animParameters), transTargetRatio(0.0f), transitionTime(0.2f), transitiionEase(HashiTaku::EaseKind::Linear), interpolateKind(AnimInterpolateKind::CrossFade)
{
}

bool AnimTransitionArrow::CheckTransition()
{
	for (auto& pCondition : conditionList)	// 全ての条件が達成しているなら
	{
		if (!pCondition->IsCondition())	return false;
	}

	return true;
}

void AnimTransitionArrow::AddCondition(const AnimParam::conditionValType& _val, const std::string& _name)
{
	// 遷移条件を変数から作成し、追加
	if (std::unique_ptr<TransCondition_Base> pCon = TransConditionCreater::Create(_val, _name))
	{
		conditionList.push_back(std::move(pCon));
	}
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
	transitiionEase = _easeKind;
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
	return transitiionEase;
}

HashiTaku::AnimInterpolateKind AnimTransitionArrow::GetInterpolateKind() const
{
	return interpolateKind;
}

nlohmann::json AnimTransitionArrow::Save()
{
	nlohmann::json data;

	// 遷移条件
	nlohmann::json conditionDataList;
	for (auto& pCond : conditionList)
	{
		nlohmann::json conData;
		conData["condition"] = pCond->Save();

		// 対応型と参照しているパラメータ名をセーブ
		conData["typeKind"] = pCond->GetTypeKind();
		conData["paramName"] = pCond->GetReferenceParamName();
		conditionDataList.push_back(conData);
	}
	data["conditionList"] = conditionDataList;

	data["transTime"] = transitionTime;
	data["targetRatio"] = transTargetRatio;
	data["transEase"] = transTargetRatio;
	data["interpolate"] = interpolateKind;

	return data;
}

void AnimTransitionArrow::Load(const nlohmann::json& _data)
{
	// 遷移条件
	nlohmann::json conditionDataList;
	if (HashiTaku::LoadJsonDataArray("conditionList", conditionDataList, _data))
	{
		for (auto& conditionData : conditionDataList)
		{
			std::string paramRefernceName;
			if (!HashiTaku::LoadJsonString("paramName", paramRefernceName, conditionData))
				continue;
			
			// 名前からアドレスを取得
			const AnimParam::conditionValType& value = *pAnimParameters->GetValueAddress(paramRefernceName);
			const std::string& name = *pAnimParameters->GetNameAddress(paramRefernceName);

			// 作成する
			std::unique_ptr<TransCondition_Base> pCondition = TransConditionCreater::Create(value, name);

			// 遷移条件自体のロード
			nlohmann::json conditionParamData;
			if (HashiTaku::LoadJsonData("condition", conditionParamData, conditionData))
				pCondition->Load(conditionParamData);

			conditionList.push_back(std::move(pCondition));	// 追加
		}
	}
	
	HashiTaku::LoadJsonFloat("transTime", transitionTime, _data);
	HashiTaku::LoadJsonFloat("targetRatio", transTargetRatio, _data);
	HashiTaku::LoadJsonEnum<HashiTaku::EaseKind>("transEase", transitiionEase, _data);
	HashiTaku::LoadJsonEnum<HashiTaku::AnimInterpolateKind>("interpolate", interpolateKind, _data);
}

void AnimTransitionArrow::ImGuiSetInterpolate()
{
#ifdef EDIT
	int selectTransition = static_cast<int>(interpolateKind);

	ImGui::Text("Transition");

	int id = static_cast<int>(AnimInterpolateKind::CrossFade);
	ImGui::RadioButton("CrossFade", &selectTransition, id);

	ImGui::SameLine();

	id = static_cast<int>(AnimInterpolateKind::Inertialization);
	ImGui::RadioButton("Inertialization", &selectTransition, id);

	interpolateKind = static_cast<AnimInterpolateKind>(selectTransition);
#endif // EDIT
}

void AnimTransitionArrow::ImGuiSetCondistion()
{
#ifdef EDIT
	for (auto& pCondtion : conditionList)	// リスト内の遷移条件
	{
		pCondtion->ImGuiCall();
	}
	ImGuiMethod::LineSpaceMid();

	// 新たに作成
	if (pAnimParameters->GetParameterCnt() == 0) return;	// パラメータが0個なら表示しない

	ImGui::Text("Add Condition");

	// 全てのパラメータ名を取得
	std::vector<const std::string*> paramNameList;
	pAnimParameters->GetNameList(paramNameList);

	// 条件を新規作成
	ImGuiMethod::ComboBox("##paramName", selectParamName, paramNameList);
	if (ImGui::Button("+"))
	{
		// 名前から名前の参照と値の参照を取得
		const AnimParam::conditionValType* paramVal = pAnimParameters->GetValueAddress(selectParamName);
		const std::string* paramName = pAnimParameters->GetNameAddress(selectParamName);

		if (paramVal && paramName)
			AddCondition(*paramVal, *paramName);	// 作成
	}
#endif // EDIT
}

void AnimTransitionArrow::ImGuiSetting()
{
	ImGui::DragFloat("transRatio", &transTargetRatio, 0.01f, 0.0f, 1.0f);	// 遷移先割合
	ImGui::DragFloat("transitionTime", &transitionTime, 0.01f, 0.0f, 10.0f);	// 遷移時間
	HashiTaku::Easing::ImGuiSelect(transitiionEase);	// 遷移イージング

	// アニメーション遷移種類
	ImGuiSetInterpolate();

	// 遷移条件
	if (ImGuiMethod::TreeNode("Condition"))
	{
		ImGuiSetCondistion();
		ImGui::TreePop();
	}
}

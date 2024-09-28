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
	for (auto& pCondition : conditionList)	// �S�Ă̏������B�����Ă���Ȃ�
	{
		if (!pCondition->IsCondition())	return false;
	}

	return true;
}

void AnimTransitionArrow::AddCondition(const AnimParam::conditionValType& _val, const std::string& _name)
{
	// �J�ڏ�����ϐ�����쐬���A�ǉ�
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
	assert(pToNode != nullptr && "�J�ڐ�̃m�[�h���ݒ肳��Ă��܂���");
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

	// �J�ڏ���
	nlohmann::json conditionDataList;
	for (auto& pCond : conditionList)
	{
		nlohmann::json conData;
		conData["condition"] = pCond->Save();

		// �Ή��^�ƎQ�Ƃ��Ă���p�����[�^�����Z�[�u
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
	// �J�ڏ���
	nlohmann::json conditionDataList;
	if (HashiTaku::LoadJsonDataArray("conditionList", conditionDataList, _data))
	{
		for (auto& conditionData : conditionDataList)
		{
			std::string paramRefernceName;
			if (!HashiTaku::LoadJsonString("paramName", paramRefernceName, conditionData))
				continue;
			
			// ���O����A�h���X���擾
			const AnimParam::conditionValType& value = *pAnimParameters->GetValueAddress(paramRefernceName);
			const std::string& name = *pAnimParameters->GetNameAddress(paramRefernceName);

			// �쐬����
			std::unique_ptr<TransCondition_Base> pCondition = TransConditionCreater::Create(value, name);

			// �J�ڏ������̂̃��[�h
			nlohmann::json conditionParamData;
			if (HashiTaku::LoadJsonData("condition", conditionParamData, conditionData))
				pCondition->Load(conditionParamData);

			conditionList.push_back(std::move(pCondition));	// �ǉ�
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
	for (auto& pCondtion : conditionList)	// ���X�g���̑J�ڏ���
	{
		pCondtion->ImGuiCall();
	}
	ImGuiMethod::LineSpaceMid();

	// �V���ɍ쐬
	if (pAnimParameters->GetParameterCnt() == 0) return;	// �p�����[�^��0�Ȃ�\�����Ȃ�

	ImGui::Text("Add Condition");

	// �S�Ẵp�����[�^�����擾
	std::vector<const std::string*> paramNameList;
	pAnimParameters->GetNameList(paramNameList);

	// ������V�K�쐬
	ImGuiMethod::ComboBox("##paramName", selectParamName, paramNameList);
	if (ImGui::Button("+"))
	{
		// ���O���疼�O�̎Q�Ƃƒl�̎Q�Ƃ��擾
		const AnimParam::conditionValType* paramVal = pAnimParameters->GetValueAddress(selectParamName);
		const std::string* paramName = pAnimParameters->GetNameAddress(selectParamName);

		if (paramVal && paramName)
			AddCondition(*paramVal, *paramName);	// �쐬
	}
#endif // EDIT
}

void AnimTransitionArrow::ImGuiSetting()
{
	ImGui::DragFloat("transRatio", &transTargetRatio, 0.01f, 0.0f, 1.0f);	// �J�ڐ抄��
	ImGui::DragFloat("transitionTime", &transitionTime, 0.01f, 0.0f, 10.0f);	// �J�ڎ���
	HashiTaku::Easing::ImGuiSelect(transitiionEase);	// �J�ڃC�[�W���O

	// �A�j���[�V�����J�ڎ��
	ImGuiSetInterpolate();

	// �J�ڏ���
	if (ImGuiMethod::TreeNode("Condition"))
	{
		ImGuiSetCondistion();
		ImGui::TreePop();
	}
}

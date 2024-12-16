#include "pch.h"
#include "AnimTransitionArrow.h"
#include "AnimationNode_Base.h"
#include "TransConditionCreater.h"
#include "AnimParamRemoveObserver.h"

namespace HashiTaku
{
	AnimTransitionArrow::AnimTransitionArrow(AnimationNode_Base& _toNode, AnimationParameters& _animParameters) :
		pToNode(&_toNode),
		pAnimParameters(&_animParameters),
		exitRatio(1.0f),
		transTargetRatio(0.0f),
		transitionTime(0.2f),
		priority(0),
		transitiionEase(EaseKind::Linear),
		interpolateKind(AnimInterpolateKind::CrossFade),
		isHasExitRatio(false)
	{
		// �A�j���[�V�����p�����[�^�폜�I�u�U�[�o�[���쐬�A�T�u�W�F�N�g�ɒǉ�����
		pRemoveParamObserver = std::make_unique<AnimParam::AnimParamObserver>(*this, "Arrow_RemoveParam");
		pAnimParameters->GetParamRemoveSubject().AddObserver(*pRemoveParamObserver);
	}


	void AnimTransitionArrow::AddCondition(const AnimParam::conditionValType& _val, const std::string& _name)
	{
		// �J�ڏ�����ϐ�����쐬���A�ǉ�
		if (std::unique_ptr<TransCondition_Base> pCon = TransConditionCreater::Create(_val, _name))
		{
			conditionList.push_back(std::move(pCon));
		}
	}

	void AnimTransitionArrow::RemoveCondition(const std::string& _condParamName)
	{
		// ���O��T���č폜����
		for (auto itr = conditionList.begin(); itr != conditionList.end();)
		{
			if (_condParamName == (*itr)->GetReferenceParamName())
			{
				itr = conditionList.erase(itr);
			}
			else
				itr++;
		}
	}

	void AnimTransitionArrow::ReNameCondition(const std::string& _prevName, const std::string& _afterName)
	{
		// �ȑO�̖��O�̃A�h���X��V�������O�̃A�h���X�ɕς���
		if (const std::string* pAddres = pAnimParameters->GetNameAddress(_afterName))
		{
			for (auto& pCond : conditionList)
			{
				if (_prevName == pCond->GetReferenceParamName())
					pCond->SetReferenceParamName(*pAddres);

			}
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

	void AnimTransitionArrow::SetPriority(int _priority)
	{
		priority = _priority;
	}

	void AnimTransitionArrow::SetEaseKind(EaseKind _easeKind)
	{
		transitiionEase = _easeKind;
	}

	void AnimTransitionArrow::SeInterpolateKind(AnimInterpolateKind _interpolateKind)
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

	float AnimTransitionArrow::GetExitRatio() const
	{
		return exitRatio;
	}

	int AnimTransitionArrow::GetPriority() const
	{
		return priority;
	}

	bool AnimTransitionArrow::GetIsHasExit() const
	{
		return isHasExitRatio;
	}

	const std::list<std::unique_ptr<TransCondition_Base>>& AnimTransitionArrow::GetConditionList() const
	{
		return conditionList;
	}

	EaseKind AnimTransitionArrow::GetEaseKind() const
	{
		return transitiionEase;
	}

	AnimInterpolateKind AnimTransitionArrow::GetInterpolateKind() const
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
		data["isHaveExit"] = isHasExitRatio;
		data["exitRatio"] = exitRatio;
		data["targetRatio"] = transTargetRatio;
		data["priority"] = priority;
		data["transEase"] = transTargetRatio;
		data["interpolate"] = interpolateKind;

		return data;
	}

	void AnimTransitionArrow::Load(const nlohmann::json& _data)
	{
		// �J�ڏ���
		nlohmann::json conditionDataList;
		if (LoadJsonDataArray("conditionList", conditionDataList, _data))
		{
			for (auto& conditionData : conditionDataList)
			{
				std::string paramRefernceName;
				if (!LoadJsonString("paramName", paramRefernceName, conditionData))
					continue;

				// ���O����A�h���X���擾
				const AnimParam::conditionValType& value = *pAnimParameters->GetValueAddress(paramRefernceName);
				const std::string& name = *pAnimParameters->GetNameAddress(paramRefernceName);

				// �쐬����
				std::unique_ptr<TransCondition_Base> pCondition = TransConditionCreater::Create(value, name);

				// �J�ڏ������̂̃��[�h
				nlohmann::json conditionParamData;
				if (LoadJsonData("condition", conditionParamData, conditionData))
					pCondition->Load(conditionParamData);

				conditionList.push_back(std::move(pCondition));	// �ǉ�
			}
		}

		LoadJsonFloat("transTime", transitionTime, _data);
		LoadJsonBoolean("isHaveExit", isHasExitRatio, _data);
		LoadJsonFloat("exitRatio", exitRatio, _data);
		LoadJsonFloat("targetRatio", transTargetRatio, _data);
		LoadJsonInteger("priority", priority, _data);
		LoadJsonEnum<EaseKind>("transEase", transitiionEase, _data);
		LoadJsonEnum<AnimInterpolateKind>("interpolate", interpolateKind, _data);
	}

	void AnimTransitionArrow::AcceptAnimParamData(const AnimParam::NotificationData& _notifyData)
	{
		using enum AnimParam::AnimParamObseveEvent;

		switch (_notifyData.eventType)
		{
		case Remove:
			RemoveCondition(*_notifyData.pDeleteName);
			break;

		case Rename:
			break;

		default:
			break;
		}
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
		u_int loop = 0;
		for (auto condItr = conditionList.begin(); condItr != conditionList.end();)	// ���X�g���̑J�ڏ���
		{
			loop++;
			ImGui::PushID(loop);

			(*condItr)->ImGuiCall();
			ImGui::SameLine();
			bool isDelete = ImGui::Button("X");

			if (isDelete)
				condItr = conditionList.erase(condItr);
			else
				condItr++;

			ImGui::PopID();
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

	void AnimTransitionArrow::ImGuiDebug()
	{
		ImGuiMethod::PushItemSmallWidth();
		ImGui::DragFloat("TransitionTime", &transitionTime, 0.01f, 0.0f, 10.0f);	// �J�ڎ���
		ImGui::Checkbox("HasExitTime", &isHasExitRatio);	// exit�t���O
		ImGui::DragFloat("ExitRatio", &exitRatio, 0.01f, 0.0f, 1.0f);	// �J�ڐ抄��
		ImGui::DragFloat("TargetRatio", &transTargetRatio, 0.01f, 0.0f, 1.0f);	// �J�ڐ抄��
		Easing::ImGuiSelect(transitiionEase);	// �J�ڃC�[�W���O
		ImGui::PopItemWidth();

		// �A�j���[�V�����J�ڎ��
		ImGuiSetInterpolate();

		// �J�ڏ���
		if (ImGuiMethod::TreeNode("Condition"))
		{
			ImGuiSetCondistion();
			ImGui::TreePop();
		}
	}
}
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
		// アニメーションパラメータ削除オブザーバーを作成、サブジェクトに追加する
		pRemoveParamObserver = std::make_unique<AnimParam::AnimParamObserver>(*this, "Arrow_RemoveParam");
		pAnimParameters->GetParamRemoveSubject().AddObserver(*pRemoveParamObserver);
	}


	void AnimTransitionArrow::AddCondition(const AnimParam::conditionValType& _val, const std::string& _name)
	{
		// 遷移条件を変数から作成し、追加
		if (std::unique_ptr<TransCondition_Base> pCon = TransConditionCreater::Create(_val, _name))
		{
			conditionList.push_back(std::move(pCon));
		}
	}

	void AnimTransitionArrow::RemoveCondition(const std::string& _condParamName)
	{
		// 名前を探して削除する
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
		// 以前の名前のアドレスを新しい名前のアドレスに変える
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
		// 遷移条件
		nlohmann::json conditionDataList;
		if (LoadJsonDataArray("conditionList", conditionDataList, _data))
		{
			for (auto& conditionData : conditionDataList)
			{
				std::string paramRefernceName;
				if (!LoadJsonString("paramName", paramRefernceName, conditionData))
					continue;

				// 名前からアドレスを取得
				const AnimParam::conditionValType& value = *pAnimParameters->GetValueAddress(paramRefernceName);
				const std::string& name = *pAnimParameters->GetNameAddress(paramRefernceName);

				// 作成する
				std::unique_ptr<TransCondition_Base> pCondition = TransConditionCreater::Create(value, name);

				// 遷移条件自体のロード
				nlohmann::json conditionParamData;
				if (LoadJsonData("condition", conditionParamData, conditionData))
					pCondition->Load(conditionParamData);

				conditionList.push_back(std::move(pCondition));	// 追加
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
		for (auto condItr = conditionList.begin(); condItr != conditionList.end();)	// リスト内の遷移条件
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

	void AnimTransitionArrow::ImGuiDebug()
	{
		ImGuiMethod::PushItemSmallWidth();
		ImGui::DragFloat("TransitionTime", &transitionTime, 0.01f, 0.0f, 10.0f);	// 遷移時間
		ImGui::Checkbox("HasExitTime", &isHasExitRatio);	// exitフラグ
		ImGui::DragFloat("ExitRatio", &exitRatio, 0.01f, 0.0f, 1.0f);	// 遷移先割合
		ImGui::DragFloat("TargetRatio", &transTargetRatio, 0.01f, 0.0f, 1.0f);	// 遷移先割合
		Easing::ImGuiSelect(transitiionEase);	// 遷移イージング
		ImGui::PopItemWidth();

		// アニメーション遷移種類
		ImGuiSetInterpolate();

		// 遷移条件
		if (ImGuiMethod::TreeNode("Condition"))
		{
			ImGuiSetCondistion();
			ImGui::TreePop();
		}
	}
}
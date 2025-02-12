#include "pch.h"
#include "IntTransCondition.h"

namespace HashiTaku
{
#ifdef EDIT
	std::vector<std::string> IntTransCondition::judgeTypeStrings =
	{
		"Greater",
		"Less",
		"Equal",
		"NotEqual"
	};
#endif // EDIT


	IntTransCondition::IntTransCondition(const std::string& _parameterName)
		: TransCondition_Base(_parameterName, AnimParam::TypeKind::Int), compareVal(0), judgeType(JudgeType::Greater)
	{
	}

	bool IntTransCondition::IsCondition(AnimParam::conditionValType _checkValue) const
	{
		int* pIntVal = std::get_if<int>(&_checkValue);
		assert(pIntVal && "int型でありません");

		// 条件達成フラグ
		bool isAchieve = false;

		switch (judgeType)
		{
		case JudgeType::Greater:
			isAchieve = *pIntVal > compareVal;
			break;

		case JudgeType::Less:
			isAchieve = *pIntVal < compareVal;
			break;

		case JudgeType::Equal:
			isAchieve = *pIntVal == compareVal;
			break;

		case JudgeType::NotEqual:
			isAchieve = *pIntVal != compareVal;
			break;

		default:
			assert(!"判別タイプが不正な値です");
			break;
		}

		return isAchieve;
	}

	json IntTransCondition::Save()
	{
		json data;
		data["iCompare"] = compareVal;
		data["iJudgeType"] = judgeType;
		return data;
	}

	void IntTransCondition::Load(const json& _data)
	{
		LoadJsonInteger("iCompare", compareVal, _data);
		LoadJsonEnum<JudgeType>("iJudgeType", judgeType, _data);
	}

	void IntTransCondition::ImGuiDebug()
	{
#ifdef EDIT
		TransCondition_Base::ImGuiDebug();
		ImGui::SameLine();

		ImGuiMethod::PushItemSmallWidth();
		// タイプを変換する
		u_int id = static_cast<u_int>(judgeType);
		ImGuiMethod::ComboBox("##intTrans", id, judgeTypeStrings);
		judgeType = static_cast<JudgeType>(id);
		ImGui::SameLine();
		// 比較定数
		ImGui::DragInt("##compare_Int", &compareVal);
		ImGui::PopItemWidth();
#endif
	}
}
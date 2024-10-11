#include "pch.h"
#include "FloatTransCondition.h"

#ifdef EDIT
std::vector<std::string> FloatTransCondition::judgeTypeStrings =
{
	"Greater",
	"Less",
	"Equal"
};
#endif // EDIT

FloatTransCondition::FloatTransCondition(const std::string& _parameterName)
	: TransCondition_Base(_parameterName, HashiTaku::AnimParam::TypeKind::Float), compareVal(0.0f), judgeType(JudgeType::Greater)
{
}

bool FloatTransCondition::IsCondition(HashiTaku::AnimParam::conditionValType _checkValue) const
{
	float* pFloatVal = std::get_if<float>(&_checkValue);
	assert(pFloatVal && "float型でありません");

	// 条件達成フラグ
	bool isAchieve = false;

	switch (judgeType)
	{
	case JudgeType::Greater:
		isAchieve = *pFloatVal > compareVal;
		break;

	case JudgeType::Less:
		isAchieve = *pFloatVal < compareVal;
		break;

	case JudgeType::Equal:
		isAchieve = abs(*pFloatVal - compareVal) <= Mathf::epsilon;
		break;

	default:
		assert(!"判別タイプが不正な値です");
		break;
	}

	return isAchieve;
}

nlohmann::json FloatTransCondition::Save()
{
	nlohmann::json data;
	data["fCompare"] = compareVal;
	data["fJudgeType"] = judgeType;
	return data;
}

void FloatTransCondition::Load(const nlohmann::json& _data)
{
	HashiTaku::LoadJsonFloat("fCompare", compareVal, _data);
	HashiTaku::LoadJsonEnum<JudgeType>("fJudgeType", judgeType, _data);
}

void FloatTransCondition::ImGuiSetting()
{
#ifdef EDIT
	TransCondition_Base::ImGuiSetting();
	ImGui::SameLine();

	ImGuiMethod::PushItemWidth();

	// タイプを変換する
	u_int id = static_cast<u_int>(judgeType);
	ImGuiMethod::ComboBox("##floatTrans", id, judgeTypeStrings);
	judgeType = static_cast<JudgeType>(id);
	ImGui::SameLine();

	// 比較定数
	ImGui::DragFloat("##compare", &compareVal, 0.1f);

	ImGui::PopItemWidth();
#endif // EDIT
}

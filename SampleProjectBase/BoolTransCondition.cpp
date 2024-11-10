#include "pch.h"
#include "BoolTransCondition.h"

#ifdef EDIT
std::vector<std::string> BoolTransCondition::judgeTypeStrings =
{
	"True",
	"False"
};
#endif // EDIT

BoolTransCondition::BoolTransCondition(const std::string& _parameterName)
	: TransCondition_Base(_parameterName, HashiTaku::AnimParam::TypeKind::Bool), judgeType(JudgeType::True)
{
}

bool BoolTransCondition::IsCondition(HashiTaku::AnimParam::conditionValType _checkValue) const
{
	bool* pIsCheckBool = std::get_if<bool>(&_checkValue);
	assert(pIsCheckBool && "bool型でありません");

	// 条件達成フラグ
	bool isAchieve = false;

	switch (judgeType)
	{
	case JudgeType::True:
		isAchieve = *pIsCheckBool == true;
		break;

	case JudgeType::False:
		isAchieve = *pIsCheckBool == false;
		break;

	default:
		assert(!"判別タイプが不正な値です");
		break;
	}

	return isAchieve;
}

nlohmann::json BoolTransCondition::Save()
{
	nlohmann::json data;
	data["bJudgeType"] = judgeType;
	return data;
}

void BoolTransCondition::Load(const nlohmann::json& _data)
{
	HashiTaku::LoadJsonEnum<JudgeType>("bJudgeType", judgeType, _data);
}

void BoolTransCondition::ImGuiDebug()
{
#ifdef EDIT
	TransCondition_Base::ImGuiDebug();
	ImGui::SameLine();
	ImGuiMethod::PushItemSmallWidth();
	// タイプを変換する
	u_int id = static_cast<u_int>(judgeType);
	ImGuiMethod::ComboBox("##compare_bool", id, judgeTypeStrings);
	judgeType = static_cast<JudgeType>(id);
	ImGui::PopItemWidth();
#endif // EDIT
}

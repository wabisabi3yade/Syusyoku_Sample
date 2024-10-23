#include "pch.h"
#include "IntTransCondition.h"

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
	: TransCondition_Base(_parameterName, HashiTaku::AnimParam::TypeKind::Int), compareVal(0), judgeType(JudgeType::Greater)
{
}

bool IntTransCondition::IsCondition(HashiTaku::AnimParam::conditionValType _checkValue) const
{
	int* pIntVal = std::get_if<int>(&_checkValue);
	assert(pIntVal && "int�^�ł���܂���");

	// �����B���t���O
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
		assert(!"���ʃ^�C�v���s���Ȓl�ł�");
		break;
	}

	return isAchieve;
}

nlohmann::json IntTransCondition::Save()
{
	nlohmann::json data;
	data["iCompare"] = compareVal;
	data["iJudgeType"] = judgeType;
	return data;
}

void IntTransCondition::Load(const nlohmann::json& _data)
{
	HashiTaku::LoadJsonInteger("iCompare", compareVal, _data);
	HashiTaku::LoadJsonEnum<JudgeType>("iJudgeType", judgeType, _data);
}

void IntTransCondition::ImGuiSetting()
{
#ifdef EDIT
	TransCondition_Base::ImGuiSetting();
	ImGui::SameLine();

	ImGuiMethod::PushItemSmallWidth();
	// �^�C�v��ϊ�����
	u_int id = static_cast<u_int>(judgeType);
	ImGuiMethod::ComboBox("##intTrans", id, judgeTypeStrings);
	judgeType = static_cast<JudgeType>(id);
	ImGui::SameLine();
	// ��r�萔
	ImGui::DragInt("##compare_Int", &compareVal);
	ImGui::PopItemWidth();
#endif
}

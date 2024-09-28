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


IntTransCondition::IntTransCondition(const int& _parameter, const std::string& _parameterName)
	: TransCondition_Base(_parameterName, HashiTaku::AnimParam::TypeKind::Int), pParameter(&_parameter), compareVal(0), judgeType(JudgeType::Greater)
{
}

bool IntTransCondition::IsCondition() const
{
	// �����B���t���O
	bool isAchieve = false;

	switch (judgeType)
	{
	case JudgeType::Greater:
		isAchieve = *pParameter > compareVal;
		break;

	case JudgeType::Less:
		isAchieve = *pParameter < compareVal;
		break;

	case JudgeType::Equal:
		isAchieve = *pParameter == compareVal;
		break;

	case JudgeType::NotEqual:
		isAchieve = *pParameter != compareVal;
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

	ImGuiMethod::PushItemWidth();
	// �^�C�v��ϊ�����
	u_int id = static_cast<u_int>(judgeType);
	ImGuiMethod::ComboBox("##intTrans", id, judgeTypeStrings);
	judgeType = static_cast<JudgeType>(id);

	// ��r�萔
	ImGui::DragInt("##compare", &compareVal);
	ImGui::PopItemWidth();
#endif
}

#include "pch.h"
#include "BoolTransCondition.h"

#ifdef EDIT
std::vector<std::string> BoolTransCondition::judgeTypeStrings =
{
	"True",
	"False"
};
#endif // EDIT

BoolTransCondition::BoolTransCondition(const bool& _parameter, const std::string& _parameterName)
	: TransCondition_Base(_parameterName, HashiTaku::AnimParam::TypeKind::Bool), pParameter(&_parameter), judgeType(JudgeType::True)
{
}

bool BoolTransCondition::IsCondition() const
{
	// �����B���t���O
	bool isAchieve = false;

	switch (judgeType)
	{
	case JudgeType::True:
		isAchieve = *pParameter == true;
		break;

	case JudgeType::False:
		isAchieve = *pParameter == false;
		break;

	default:
		assert(!"���ʃ^�C�v���s���Ȓl�ł�");
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

void BoolTransCondition::ImGuiSetting()
{
#ifdef EDIT
	TransCondition_Base::ImGuiSetting();
	ImGui::SameLine();
	ImGuiMethod::PushItemWidth();
	// �^�C�v��ϊ�����
	u_int id = static_cast<u_int>(judgeType);
	ImGuiMethod::ComboBox("##boolTrans", id, judgeTypeStrings);
	judgeType = static_cast<JudgeType>(id);
	ImGui::PopItemWidth();
#endif // EDIT
}

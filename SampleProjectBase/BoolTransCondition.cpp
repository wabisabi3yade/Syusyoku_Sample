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
	assert(pIsCheckBool && "bool�^�ł���܂���");

	// �����B���t���O
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

void BoolTransCondition::ImGuiDebug()
{
#ifdef EDIT
	TransCondition_Base::ImGuiDebug();
	ImGui::SameLine();
	ImGuiMethod::PushItemSmallWidth();
	// �^�C�v��ϊ�����
	u_int id = static_cast<u_int>(judgeType);
	ImGuiMethod::ComboBox("##compare_bool", id, judgeTypeStrings);
	judgeType = static_cast<JudgeType>(id);
	ImGui::PopItemWidth();
#endif // EDIT
}

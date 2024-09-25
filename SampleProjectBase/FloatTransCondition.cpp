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

FloatTransCondition::FloatTransCondition(float& _parameter)
	: pParameter(&_parameter), compareVal(0.0f), judgeType(JudgeType::Greater)
{
}

bool FloatTransCondition::IsCondition() const
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
		isAchieve = abs(*pParameter - compareVal) <= Mathf::epsilon;
		break;

	default:
		assert(!"���ʃ^�C�v���s���Ȓl�ł�");
		break;
	}

	return isAchieve;
}

void FloatTransCondition::ImGuiSetting()
{
#ifdef EDIT
	ImGui::DragFloat("compare", &compareVal, 0.1f);
	
	// �^�C�v��ϊ�����
	u_int id = static_cast<u_int>(judgeType);
	ImGuiMethod::PushRandID();
	ImGuiMethod::ComboBox("", id, judgeTypeStrings);
	ImGui::PopID();
	judgeType = static_cast<JudgeType>(id);
#endif // EDIT
}

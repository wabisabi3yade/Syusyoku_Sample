#pragma once
#include "TransCondition_Base.h"

/// @brief float�^�ϐ������āA�J�ڏ����B�����Ă��邩
class FloatTransCondition : public TransCondition_Base
{
public:
	/// @brief float�^���������������߂̔���
	enum class JudgeType
	{
		Greater,	// ����
		Less,	// ��艺
		Equal	// ������
	};

#ifdef EDIT
	/// @brief ����^�C�v�̕�����
	static std::vector<std::string> judgeTypeStrings;
#endif // EDIT
private:
	/// @brief �J�ڏ����ŎQ�Ƃ���float�ϐ�
	float* pParameter;

	/// @brief ��r����l
	float compareVal;

	/// @brief ���ʃ^�C�v
	JudgeType judgeType;
public:
	FloatTransCondition(float& _parameter);
	~FloatTransCondition() {}

	bool IsCondition()const override;
private:

	void ImGuiSetting() override;
};


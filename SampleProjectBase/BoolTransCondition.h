#pragma once
#include "TransCondition_Base.h"

/// @brief bool�^�ϐ������āA�J�ڏ����B�����Ă��邩
class BoolTransCondition : public TransCondition_Base
{
	/// @brief bool�^���������������߂̔���
	enum class JudgeType
	{
		True,
		False
	};

#ifdef EDIT
	/// @brief ����^�C�v�̕�����
	static std::vector<std::string> judgeTypeStrings;
#endif
private:
	// @brief �J�ڏ����ŎQ�Ƃ���bool�ϐ�
	const bool* pParameter;

	/// @brief ���ʃ^�C�v
	JudgeType judgeType;
public:
	BoolTransCondition(const bool& _parameter, const std::string& _parameterName);
	~BoolTransCondition() {}

	bool IsCondition()const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void ImGuiSetting() override;
};


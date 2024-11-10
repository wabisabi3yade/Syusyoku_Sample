#pragma once
#include "TransCondition_Base.h"

/// @brief int�^�ϐ������āA�J�ڏ����B�����Ă��邩
class IntTransCondition : public TransCondition_Base
{
public:
	/// @brief int�^���������������߂̔���
	enum class JudgeType
	{
		Greater,	// ����
		Less,	// ��艺
		Equal,	// ������
		NotEqual	// �������Ȃ�
	};

#ifdef EDIT
	/// @brief ����^�C�v�̕�����
	static std::vector<std::string> judgeTypeStrings;
#endif
private:
	/// @brief ��r����l
	int compareVal;

	JudgeType judgeType;
public:
	IntTransCondition(const std::string& _parameterName);
	~IntTransCondition() {}

	bool IsCondition(HashiTaku::AnimParam::conditionValType _checkValue) const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void ImGuiDebug() override;
};


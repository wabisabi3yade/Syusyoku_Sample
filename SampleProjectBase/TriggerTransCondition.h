#pragma once
#include "TransCondition_Base.h"

/// @brief �g���K�[�^�ϐ������āA�J�ڏ����B�����Ă��邩
class TriggerTransCondition : public TransCondition_Base
{
public:
	TriggerTransCondition(const std::string& _parameterName);
	~TriggerTransCondition() {}

	bool IsCondition(HashiTaku::AnimParam::conditionValType _checkValue)const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
};


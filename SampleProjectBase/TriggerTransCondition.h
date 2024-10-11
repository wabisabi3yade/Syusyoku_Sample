#pragma once
#include "TransCondition_Base.h"

/// @brief トリガー型変数を見て、遷移条件達成しているか
class TriggerTransCondition : public TransCondition_Base
{
public:
	TriggerTransCondition(const std::string& _parameterName);
	~TriggerTransCondition() {}

	bool IsCondition(HashiTaku::AnimParam::conditionValType _checkValue)const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
};


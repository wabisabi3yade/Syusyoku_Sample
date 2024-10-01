#pragma once
#include "TransCondition_Base.h"

/// @brief トリガー型変数を見て、遷移条件達成しているか
class TriggerTransCondition : public TransCondition_Base
{
	const TriggerType* pParameter;

public:
	TriggerTransCondition(const TriggerType& _parameter, const std::string& _parameterName);
	~TriggerTransCondition() {}

	bool IsCondition()const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
};


#pragma once
#include "TransCondition_Base.h"

/// @brief ƒgƒŠƒK[Œ^•Ï”‚ğŒ©‚ÄA‘JˆÚğŒ’B¬‚µ‚Ä‚¢‚é‚©
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


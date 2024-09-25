#pragma once
#include "TransCondition_Base.h"

/// @brief boolŒ^•Ï”‚ğŒ©‚ÄA‘JˆÚğŒ’B¬‚µ‚Ä‚¢‚é‚©
class BoolTransCondition : public TransCondition_Base
{
	// @brief ‘JˆÚğŒ‚ÅQÆ‚·‚ébool•Ï”
	bool* pParameter;

public:
	BoolTransCondition(bool& _parameter);
	~BoolTransCondition() {}

	bool IsCondition()const override;
private:

	void ImGuiSetting() override;
};


#pragma once
#include "TransCondition_Base.h"

/// @brief intŒ^•Ï”‚ğŒ©‚ÄA‘JˆÚğŒ’B¬‚µ‚Ä‚¢‚é‚©
class IntTransCondition : public TransCondition_Base
{
	/// @brief ‘JˆÚğŒ‚ÅQÆ‚·‚éint•Ï”
	int* pParameter;
public:
	IntTransCondition(int& _parameter);
	~IntTransCondition() {}

	bool IsCondition() const override;

private:
	void ImGuiSetting() override;
};


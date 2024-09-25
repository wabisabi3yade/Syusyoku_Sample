#pragma once
#include "TransCondition_Base.h"

/// @brief int型変数を見て、遷移条件達成しているか
class IntTransCondition : public TransCondition_Base
{
	/// @brief 遷移条件で参照するint変数
	int* pParameter;
public:
	IntTransCondition(int& _parameter);
	~IntTransCondition() {}

	bool IsCondition() const override;

private:
	void ImGuiSetting() override;
};


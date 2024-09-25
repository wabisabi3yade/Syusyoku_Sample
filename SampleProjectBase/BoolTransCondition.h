#pragma once
#include "TransCondition_Base.h"

/// @brief bool型変数を見て、遷移条件達成しているか
class BoolTransCondition : public TransCondition_Base
{
	// @brief 遷移条件で参照するbool変数
	bool* pParameter;

public:
	BoolTransCondition(bool& _parameter);
	~BoolTransCondition() {}

	bool IsCondition()const override;
private:

	void ImGuiSetting() override;
};


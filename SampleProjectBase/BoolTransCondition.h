#pragma once
#include "TransCondition_Base.h"

/// @brief bool�^�ϐ������āA�J�ڏ����B�����Ă��邩
class BoolTransCondition : public TransCondition_Base
{
	// @brief �J�ڏ����ŎQ�Ƃ���bool�ϐ�
	bool* pParameter;

public:
	BoolTransCondition(bool& _parameter);
	~BoolTransCondition() {}

	bool IsCondition()const override;
private:

	void ImGuiSetting() override;
};


#pragma once
#include "TransCondition_Base.h"

/// @brief int�^�ϐ������āA�J�ڏ����B�����Ă��邩
class IntTransCondition : public TransCondition_Base
{
	/// @brief �J�ڏ����ŎQ�Ƃ���int�ϐ�
	int* pParameter;
public:
	IntTransCondition(int& _parameter);
	~IntTransCondition() {}

	bool IsCondition() const override;

private:
	void ImGuiSetting() override;
};


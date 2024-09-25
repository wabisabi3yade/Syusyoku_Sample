#include "pch.h"
#include "IntTransCondition.h"

IntTransCondition::IntTransCondition(int& _parameter)
	: pParameter(&_parameter)
{
}

bool IntTransCondition::IsCondition() const
{
	return false;
}

void IntTransCondition::ImGuiSetting()
{
}

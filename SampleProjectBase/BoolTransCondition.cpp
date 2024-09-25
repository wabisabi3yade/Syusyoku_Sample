#include "pch.h"
#include "BoolTransCondition.h"

BoolTransCondition::BoolTransCondition(bool& _parameter)
	: pParameter(&_parameter)
{
}

bool BoolTransCondition::IsCondition() const
{
	return false;
}

void BoolTransCondition::ImGuiSetting()
{
}

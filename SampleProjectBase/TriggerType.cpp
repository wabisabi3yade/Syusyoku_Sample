#include "pch.h"
#include "TriggerType.h"

TriggerType::TriggerType()
	: isTrigger(false)
{
}

void TriggerType::SetIsTrigger()
{
	isTrigger = true;
}

void TriggerType::ResetTrigger()
{
	isTrigger = false;
}

bool TriggerType::GetIsTrigger() const
{
	return isTrigger;
}

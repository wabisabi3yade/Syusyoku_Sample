#include "pch.h"
#include "ANE_DebugLog.h"

ANE_DebugLog::ANE_DebugLog()
	: displayMessage("Default")
{
}

void ANE_DebugLog::SetMessage(const std::string& _message)
{
	displayMessage = _message;
}

std::string ANE_DebugLog::ClassNameToStr()
{
	return typeid(ANE_DebugLog).name();
}

void ANE_DebugLog::OnEvent()
{
	HASHI_DEBUG_LOG(displayMessage);
}
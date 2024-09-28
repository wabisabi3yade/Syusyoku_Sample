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

std::string ANE_DebugLog::GetTypeName() const
{
	return TYPENAME_ROUGH(ANE_DebugLog);
}

void ANE_DebugLog::ImGuiSetting()
{
#ifdef  EDIT
	AnimationNotifyEvent::ImGuiSetting();
	ImGuiMethod::EditableText("##aneDebug", displayMessage, isEditing);
#endif //  EDIT
}

nlohmann::json ANE_DebugLog::Save()
{
	auto data = AnimationNotifyEvent::Save();
	data["message"] = displayMessage;
	return data;
}

void ANE_DebugLog::Load(const nlohmann::json& _data)
{
	AnimationNotifyEvent::Load(_data);
	HashiTaku::LoadJsonString("message", displayMessage, _data);
}

void ANE_DebugLog::OnEvent()
{
	HASHI_DEBUG_LOG(displayMessage);
}
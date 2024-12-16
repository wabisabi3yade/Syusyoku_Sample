#include "pch.h"
#include "ANE_DebugLog.h"

namespace HashiTaku
{
	ANE_DebugLog::ANE_DebugLog() :
		AnimationNotifyEvent(NotifyType::ANE_DebugLog),
		displayMessage("Default")
	{
	}

	void ANE_DebugLog::SetMessage(const std::string& _message)
	{
		displayMessage = _message;
	}

	void ANE_DebugLog::ImGuiDebug()
	{
#ifdef  EDIT
		AnimationNotifyEvent::ImGuiDebug();
		ImGuiMethod::EditableText("##aneDebug", displayMessage, isEditing);
#endif //  EDIT
	}

	std::unique_ptr<AnimationNotify_Base> ANE_DebugLog::Clone()
	{
		return std::make_unique<ANE_DebugLog>(*this);
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
		LoadJsonString("message", displayMessage, _data);
	}

	void ANE_DebugLog::OnEvent()
	{
		HASHI_DEBUG_LOG(displayMessage);
	}
}
#include "pch.h"
#include "ANS_DebugLog.h"

namespace HashiTaku
{
	ANS_DebugLog::ANS_DebugLog() :
		AnimationNotifyState(NotifyType::ANE_DebugLog),
		selectId(0),
		inputString("\0")
	{
		message[0] = "Begin";
		message[1] = "Tick";
		message[2] = "End";
	}

	std::unique_ptr<AnimationNotify_Base> ANS_DebugLog::Clone()
	{
		return std::make_unique<ANS_DebugLog>(*this);
	}

	json ANS_DebugLog::Save()
	{
		auto data = AnimationNotifyState::Save();
		data["bMessage"] = message[0];
		data["tMessage"] = message[1];
		data["eMessage"] = message[2];
		return data;
	}

	void ANS_DebugLog::Load(const json& _data)
	{
		AnimationNotifyState::Load(_data);
	}

	void ANS_DebugLog::Begin()
	{
		HASHI_DEBUG_LOG(message[0]);
	}

	void ANS_DebugLog::Tick()
	{
		HASHI_DEBUG_LOG(message[1]);
	}

	void ANS_DebugLog::End()
	{
		HASHI_DEBUG_LOG(message[2]);
	}

	void ANS_DebugLog::ImGuiDebug()
	{
		AnimationNotifyState::ImGuiDebug();

		std::vector<std::string> typeName =
		{
			"Begin",
			"Tick",
			"End"
		};

		for (u_int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			ImGuiMethod::EditableText("##ansDebug", message[i], isEditing[i]);
			ImGui::PopID();
		}
	}
}
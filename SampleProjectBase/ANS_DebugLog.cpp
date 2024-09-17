#include "pch.h"
#include "ANS_DebugLog.h"

ANS_DebugLog::ANS_DebugLog()
	: selectId(0), inputString(nullptr)
{
	message[0] = "Begin";
	message[1] = "Tick";
	message[2] = "End";
}

std::string ANS_DebugLog::ClassNameToStr()
{
	return typeid(ANS_DebugLog).name();
}

void ANS_DebugLog::Begin()
{
}

void ANS_DebugLog::Tick()
{
}

void ANS_DebugLog::End()
{
}

void ANS_DebugLog::ImGuiSetting()
{
	std::vector<std::string> typeName =
	{
		"Begin",
		"Tick",
		"End"
	};

	if (ImGui::Button("Set"))
	{
		message[selectId] = inputString;
	}
	ImGui::SameLine();
	ImGui::InputText("", inputString, IM_INPUT_BUF);
	ImGui::SameLine();
	ImGuiMethod::ComboBox("message", selectId, typeName);
}

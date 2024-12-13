#include "pch.h"
#include "PlayerAttackInformation.h"

HashiTaku::PlayerAttackInformation::PlayerAttackInformation() : breakValue(1.0f)
{
}

float HashiTaku::PlayerAttackInformation::GetBreakValue() const
{
	return breakValue;
}

nlohmann::json HashiTaku::PlayerAttackInformation::Save()
{
	nlohmann::json data = AttackInformation::Save();
	data["break"] = breakValue;
	return data;
}

void HashiTaku::PlayerAttackInformation::Load(const nlohmann::json& _data)
{
	AttackInformation::Load(_data);
	LoadJsonFloat("break", breakValue, _data);
}

void HashiTaku::PlayerAttackInformation::ImGuiDebug()
{
	AttackInformation::ImGuiDebug();
	ImGui::DragFloat("Break", &breakValue, 0.01f, 0.0f, 1000.0f);
}

#include "pch.h"
#include "PlayerAttackInformation.h"

namespace HashiTaku
{
	PlayerAttackInformation::PlayerAttackInformation() : breakValue(1.0f)
	{
	}

	float PlayerAttackInformation::GetBreakValue() const
	{
		return breakValue;
	}

	nlohmann::json PlayerAttackInformation::Save()
	{
		nlohmann::json data = AttackInformation::Save();
		data["break"] = breakValue;
		return data;
	}

	void PlayerAttackInformation::Load(const nlohmann::json& _data)
	{
		AttackInformation::Load(_data);
		LoadJsonFloat("break", breakValue, _data);
	}

	void PlayerAttackInformation::ImGuiDebug()
	{
		AttackInformation::ImGuiDebug();
		ImGui::DragFloat("Break", &breakValue, 0.01f, 0.0f, 1000.0f);
	}
}
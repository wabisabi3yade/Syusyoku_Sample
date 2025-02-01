#include "pch.h"
#include "BossAttackInformation.h"

namespace HashiTaku
{
	BossAttackInformation::BossAttackInformation() :
		addBreakValueOnParry(2.0f)
	{
	}

	float BossAttackInformation::GetBreakValueOnParry() const
	{
		return addBreakValueOnParry;
	}

	json BossAttackInformation::Save()
	{
		json data = AttackInformation::Save();

		data["addBreakOnParry"] = addBreakValueOnParry;

		return data;
	}

	void BossAttackInformation::Load(const json& _data)
	{
		AttackInformation::Load(_data);

		LoadJsonFloat("addBreakOnParry", addBreakValueOnParry, _data);
	}

	void BossAttackInformation::ImGuiDebug()
	{
		AttackInformation::ImGuiDebug();

		ImGui::DragFloat("BreakValue Parry", &addBreakValueOnParry, 0.1f, 0.0f, 1000.0f);
	}
}

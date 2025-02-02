#include "pch.h"
#include "BossAttackInformation.h"

namespace HashiTaku
{
	BossAttackInformation::BossAttackInformation(IAttacker* _pAttacker) :
		AttackInformation(_pAttacker),
		addBreakValueOnParry(2.0f),
		isStunOnParry(false)
	{
	}
	void BossAttackInformation::NotifyAcceptParry(const AcceptParryInfo& _acceptInfo)
	{
		// ’Ê’m‚·‚é
		CastAcceptParry()->OnAcceptParry(_acceptInfo);
	}

	float BossAttackInformation::GetBreakValueOnParry() const
	{
		return addBreakValueOnParry;
	}

	bool BossAttackInformation::GetIsStunOnParry() const
	{
		return isStunOnParry;
	}

	json BossAttackInformation::Save()
	{
		json data = AttackInformation::Save();

		data["addBreakOnParry"] = addBreakValueOnParry;
		data["isStunOnParry"] = isStunOnParry;

		return data;
	}

	void BossAttackInformation::Load(const json& _data)
	{
		AttackInformation::Load(_data);

		LoadJsonFloat("addBreakOnParry", addBreakValueOnParry, _data);
		LoadJsonBoolean("isStunOnParry", isStunOnParry, _data);
	}

	IAttacker_AcceptParry* BossAttackInformation::CastAcceptParry()
	{
#ifdef EDIT
		return dynamic_cast<IAttacker_AcceptParry*>(pAttacker);
#endif // EDIT

		return nullptr;
	}

	void BossAttackInformation::ImGuiDebug()
	{
		AttackInformation::ImGuiDebug();
		
		ImGui::Checkbox("IsStun Parry", &isStunOnParry);
		ImGui::DragFloat("BreakValue Parry", &addBreakValueOnParry, 0.1f, 0.0f, 1000.0f);
	}
}

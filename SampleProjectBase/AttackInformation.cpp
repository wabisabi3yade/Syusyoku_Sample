#include "pch.h"
#include "AttackInformation.h"

HashiTaku::AttackInformation::AttackInformation()
	: atkDamage(0.0f), hitStopFrame(0), atkLevel(AttackLevel::Low)
{
	SetAttackLevel(AttackLevel::Low);
}

HashiTaku::AttackInformation::AttackInformation(float _atkDamage, AttackLevel _atkLevel)
	: atkDamage(_atkDamage)
{
	SetAttackLevel(_atkLevel);
}

void HashiTaku::AttackInformation::SetDamageValue(float _atkDamage)
{
	atkDamage = _atkDamage;
}

void HashiTaku::AttackInformation::SetHitStopFlame(u_int _hitStopFlame)
{
	hitStopFrame = _hitStopFlame;
}

void HashiTaku::AttackInformation::SetAttackLevel(AttackLevel _atkLevel)
{
	atkLevel = _atkLevel;
	ApplyFromAttackLevel();
}


float HashiTaku::AttackInformation::GetDamageValue() const
{
	return atkDamage;
}

u_int HashiTaku::AttackInformation::GetHitStopFlame() const
{
	return hitStopFrame;
}

HashiTaku::AttackInformation::AttackLevel HashiTaku::AttackInformation::GetAttackLevel() const
{
	return atkLevel;
}

nlohmann::json HashiTaku::AttackInformation::Save()
{
	nlohmann::json data;

	data["damage"] = atkDamage;
	data["level"] = atkLevel;

	return data;
}

void HashiTaku::AttackInformation::Load(const nlohmann::json& _data)
{
	LoadJsonFloat("damage", atkDamage, _data);
	
	if (LoadJsonEnum<AttackLevel>("level", atkLevel, _data))
	{
		SetAttackLevel(atkLevel);
	}
}

void HashiTaku::AttackInformation::ApplyFromAttackLevel()
{
	using enum HashiTaku::AttackInformation::AttackLevel;
	switch (atkLevel)
	{
	case Low:
		hitStopFrame = LOW_HITSTOP;
		break;

	case Mid:
		hitStopFrame = MID_HITSTOP;
		break;
	case High:
		hitStopFrame = HIGH_HITSTOP;
		break;
	case SuperHigh:
		hitStopFrame = SUPERHIGH_HITSTOP;
		break;
	}
}

void HashiTaku::AttackInformation::ImGuiDebug()
{
	// ƒŒƒxƒ‹
	std::vector<std::string> levelNames
	{
		"Low",
		"Middle",
		"High",
		"SuperHigh"
	};

	ImGui::Text("HSframe:%d", hitStopFrame);
	ImGui::DragFloat("AtkDamage", &atkDamage, 0.1f, 0.0f, 9999.0f);
	u_int id = static_cast<u_int>(atkLevel);
	if (ImGuiMethod::ComboBox("AtkLevel", id, levelNames))
	{
		SetAttackLevel(static_cast<AttackLevel>(id));
	}

}

#include "pch.h"
#include "AttackInformation.h"

HashiTaku::AttackInformation::AttackInformation()
	: atkDamage(0.0f), hitStopFlame(0), atkLevel(AttackLevel::Low)
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
	hitStopFlame = _hitStopFlame;
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
	return hitStopFlame;
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
		hitStopFlame = LOW_HITSTOP;
		break;

	case Mid:
		hitStopFlame = MID_HITSTOP;
		break;
	case High:
		hitStopFlame = HIGH_HITSTOP;
		break;
	case SuperHigh:
		hitStopFlame = SUPERHIGH_HITSTOP;
		break;
	}
}

void HashiTaku::AttackInformation::ImGuiDebug()
{
	// ダメージ値
	ImGui::DragFloat("AtkDamage", &atkDamage, 0.1f, 0.0f, 9999.0f);

	// レベル
	std::vector<std::string> levelNames
	{
		"Low",
		"Middle",
		"High",
		"SuperHigh"
	};
	u_int id = static_cast<u_int>(atkLevel);
	if (ImGuiMethod::ComboBox("AtkLevel", id, levelNames))
	{
		SetAttackLevel(static_cast<AttackLevel>(id));
	}

}

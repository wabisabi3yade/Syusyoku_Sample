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
}


float HashiTaku::AttackInformation::GetDamageValue() const
{
	return atkDamage;
}

u_int HashiTaku::AttackInformation::GetHitStopFlame() const
{
	return hitStopFlame;
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

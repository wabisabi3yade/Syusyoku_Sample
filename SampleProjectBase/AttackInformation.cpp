#include "pch.h"
#include "AttackInformation.h"

HashiTaku::AttackInformation::AttackInformation()
	: atkDamage(0.0f), hitStopFlame(0)
{
}

HashiTaku::AttackInformation::AttackInformation(float _atkDamage, u_int _hitStopFlame)
	: atkDamage(_atkDamage), hitStopFlame(_hitStopFlame)
{
}

float HashiTaku::AttackInformation::GetDamageValue() const
{
	return atkDamage;
}

u_int HashiTaku::AttackInformation::GetHitStopFlame() const
{
	return hitStopFlame;
}

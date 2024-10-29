#include "pch.h"
#include "AttackInformation.h"

HashiTaku::AttackInformation::AttackInformation()
	: atkDamage(0.0f)
{
}

HashiTaku::AttackInformation::AttackInformation(float _atkDamage)
	: atkDamage(_atkDamage)
{
}

float HashiTaku::AttackInformation::GetDamageValue() const
{
	return atkDamage;
}

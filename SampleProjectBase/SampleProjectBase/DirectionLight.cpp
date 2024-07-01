#include "pch.h"
#include "DirectionLight.h"

void DirectionLight::Copy(const DirectionLight& _other)
{
	direction = _other.direction;
}

DirectionLight::DirectionLight(const DirectionLight& _other) : Light(_other)
{
	if (this == &_other) return;
	Copy(_other);
}

DirectionLight& DirectionLight::operator=(const DirectionLight& _other)
{
	if (this == &_other) return *this;
	Light::operator=(_other);
	Copy(_other);
	return *this;
}

const DirectionLParameter DirectionLight::GetParameter()
{
	DirectionLParameter d;
	d.base = parameter;
	d.direction = direction;
	return d;
}

#include "pch.h"
#include "PointLight.h"

void PointLight::Copy(const PointLight& _other)
{
	range = _other.range;
}

PointLight::PointLight(const PointLight& _other) : Light(_other)
{
	if (this == &_other) return;
	Copy(_other);
	return;
}

PointLight& PointLight::operator=(const PointLight& _other)
{
	if (this == &_other) return *this;
	Light::operator=(_other);
	Copy(_other);
	return *this;
}

const PointLParameter& PointLight::GetParameter()
{
	PointLParameter p;
	p.base = parameter;
	p.range = range;

	return p;
}

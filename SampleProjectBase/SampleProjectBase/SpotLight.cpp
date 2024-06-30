#include "pch.h"
#include "SpotLight.h"

void SpotLight::Copy(const SpotLight& _other)
{
	distance = _other.distance;
	rangeAngle = _other.rangeAngle;
	direction = _other.direction;
}

SpotLight::SpotLight(const SpotLight& _other) : Light(_other)
{
	if (this == &_other) return;
	Copy(_other);
	return;
}

SpotLight& SpotLight::operator=(const SpotLight& _other)
{
	if (this == &_other) return *this;
	Light::operator=(_other);
	Copy(_other);
	return *this;
}

SpotLParameter SpotLight::GetParameter()
{
	SpotLParameter s;
	s.base = parameter;
	s.direction = direction;
	s.distance = distance;
	s.rangeAngle = s.rangeAngle;
	return s;
}

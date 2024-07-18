#include "pch.h"
#include "PointLight.h"

void PointLight::Copy(const PointLight& _other)
{
	lightRange = _other.lightRange;
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

void PointLight::SetRange(float _range)
{
	// 0–¢–ž‚É‚µ‚È‚¢
	lightRange = std::max(_range, 0.0f);
}

float PointLight::GetRange() const
{
	return lightRange;
}

PointLParameter PointLight::GetParameter()
{
	PointLParameter p;
	p.base = parameter;
	p.range = lightRange;

	return p;
}

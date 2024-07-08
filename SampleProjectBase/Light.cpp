#include "pch.h"
#include "Light.h"

using namespace DirectX::SimpleMath;
void Light::Copy(const Light& _other)
{
	parameter = _other.parameter;
	type = _other.type;
}

Light::Light(const Light& _other)
{
	if (this == &_other) return;
	Copy(_other);
	return;
}

Light& Light::operator=(const Light& _other)
{
	if (this == &_other) return *this;
	Copy(_other);
	return *this;
}
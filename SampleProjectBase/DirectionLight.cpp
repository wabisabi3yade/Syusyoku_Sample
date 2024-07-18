#include "pch.h"
#include "DirectionLight.h"

#include "Geometory.h"

using namespace DirectX::SimpleMath;
void DirectionLight::Draw()
{
	Geometory::SetPosition(parameter.position);
	Geometory::SetColor(parameter.color);

	if(GetDisplay())
	Geometory::DrawCube();
}

void DirectionLight::SetDirection(const DirectX::SimpleMath::Vector3& _dir)
{
	Vector3 d = _dir;

	// -1.0fÅ`1.0fÇÃîÕàÕì‡Ç…Ç®Ç≥ÇﬂÇÈ
	float min = -1.0f;
	float max = 1.0f;
	d.x = std::clamp<float>(d.x, min, max);
	d.y = std::clamp<float>(d.y, min, max);
	d.z = std::clamp<float>(d.z, min, max);

	direction = d;
}

DirectionLParameter DirectionLight::GetParameter()
{
	DirectionLParameter d;
	d.base = parameter;
	d.direction = direction;
	return d;
}

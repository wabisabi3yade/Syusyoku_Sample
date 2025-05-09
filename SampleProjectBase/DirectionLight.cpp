#include "pch.h"
#include "DirectionLight.h"

#include "Geometory.h"

namespace HashiTaku
{
	void DirectionLight::Draw()
	{
#ifdef EDIT
		Geometory::SetPosition(parameter.position);
		Geometory::SetColor(parameter.color);

		if (GetDisplay())
			Geometory::DrawCube();
#endif
	}

	void DirectionLight::SetDirection(const DXSimp::Vector3& _dir)
	{
		DXSimp::Vector3 d = _dir;

		// -1.0fÅ`1.0fÇÃîÕàÕì‡Ç…Ç®Ç≥ÇﬂÇÈ
		float min = -1.0f;
		float max = 1.0f;
		d.x = std::clamp<float>(d.x, min, max);
		d.y = std::clamp<float>(d.y, min, max);
		d.z = std::clamp<float>(d.z, min, max);

		direction = d;
	}

	void DirectionLight::SetAmbiemt(const DXSimp::Color& _color)
	{
		ambiemt = Vec4::Max(_color, 0.0f);
	}

	const DXSimp::Vector3& DirectionLight::GetDirection() const
	{
		return direction;
	}

	const DXSimp::Color& DirectionLight::GetAmbiemt() const
	{
		return ambiemt;
	}

	DirectionLParameter DirectionLight::GetParameter() const
	{
		DirectionLParameter d;
		d.base = parameter;
		d.direction = direction;
		d.ambeimt = ambiemt;
		return d;
	}
}
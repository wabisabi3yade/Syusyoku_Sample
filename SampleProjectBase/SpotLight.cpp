#include "pch.h"
#include "SpotLight.h"

namespace HashiTaku
{
	void SpotLight::SetDistance(float _distance)
	{
		lightDistance = std::max(_distance, 0.0f);
	}

	void SpotLight::SetRangeAngle(float _rangeAngle)
	{
		rangeAngle = std::max(_rangeAngle, 0.0f);
	}

	void SpotLight::SetDirection(const DXSimp::Vector3& _direction)
	{
		lightDir = _direction;
	}

	float SpotLight::GetDistance()
	{
		return lightDistance;
	}

	float SpotLight::GetRangeAngle()
	{
		return rangeAngle;
	}

	const DXSimp::Vector3& SpotLight::GetLightDir()
	{
		return lightDir;
	}

	SpotLParameter SpotLight::GetParameter()
	{
		SpotLParameter s;
		s.base = parameter;
		s.direction = lightDir;
		s.distance = lightDistance;
		s.rangeAngle = s.rangeAngle;
		return s;
	}
}
#pragma once
#include "Light.h"

struct SpotLParameter
{
	LightParameter base;
	float distance{5.0f};
	float rangeAngle{45.0f};
	DirectX::SimpleMath::Vector3 direction{ 0.0f, -1.0f, 0.0f };
};

class SpotLight :
	public Light
{
	// Œõ‚Ì‹——£
	float distance;
	// •úŽËŠp“x
	float rangeAngle;
	// Œõ‚Ì•ûŒü
	DirectX::SimpleMath::Vector3 direction;
	LightParameter parameter;

	void Copy(const SpotLight& _other);
public:
	SpotLight() : distance(5.0f), rangeAngle(45.0f), direction(0.0f, -1.0f, 0.0f)
	{
		type = Type::Spot;
	};

	SpotLight(const SpotLight& _other);
	~SpotLight() {};

	SpotLight& operator=(const SpotLight& _other);

	void SetDistance(float _distance) { distance = _distance; }
	void SetRangeAngle(float _rangeAngle) { rangeAngle = _rangeAngle; }

	SpotLParameter GetParameter();
};
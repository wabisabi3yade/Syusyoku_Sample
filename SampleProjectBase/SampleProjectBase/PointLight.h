#pragma once
#include "Light.h"

struct PointLParameter
{
	LightParameter base;
	float range{5.0f};
};

// ポイントライト
class PointLight :
	public Light
{
private:
	// 光の範囲
	float range;

	void Copy(const PointLight& _other);
public:
	PointLight() : range(5.0f) { type = Type::Point; };
	PointLight(const PointLight& _other);
	~PointLight(){};

	PointLight& operator=(const PointLight& _other);

	void SetRange(float _range) { range = _range; }
	PointLParameter GetParameter();
};


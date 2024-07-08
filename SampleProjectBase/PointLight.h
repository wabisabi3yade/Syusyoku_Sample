#pragma once
#include "Light.h"

struct PointLParameter
{
	LightParameter base;
	float range{5.0f};
};

// �|�C���g���C�g
class PointLight :
	public Light
{
private:
	// ���͈̔�
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


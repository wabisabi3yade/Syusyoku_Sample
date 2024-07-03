#pragma once
#include "Sphere.h"
#include "GameObject.h"

class ShaderSphere : public GameObject
{
	std::unique_ptr<Sphere> pSphereMesh;

public:
	ShaderSphere();
	~ShaderSphere(){};

	void Draw()override;

	void SetMaterial(MaterialClass& _material);
};


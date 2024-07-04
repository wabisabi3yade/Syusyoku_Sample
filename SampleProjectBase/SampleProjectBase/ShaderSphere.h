#pragma once
#include "Sphere.h"
#include "GameObject.h"

class ShaderSphere : public GameObject
{
	std::unique_ptr<Sphere> pSphereMesh;

public:
	ShaderSphere();
	~ShaderSphere(){};

	void Update() override;

	void Draw()override;

	void SetMaterial(Material& _material);
};


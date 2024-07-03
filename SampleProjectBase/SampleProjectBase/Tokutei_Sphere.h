#pragma once
#include "Sphere.h"
#include "GameObject.h"

class Tokutei_Sphere : public GameObject
{
	std::unique_ptr<Sphere> pSphereMesh;

public:
	Tokutei_Sphere();
	~Tokutei_Sphere();

	void Draw()override;
};


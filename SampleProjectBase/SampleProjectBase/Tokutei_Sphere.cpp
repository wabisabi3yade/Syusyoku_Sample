#include "pch.h"
#include "Tokutei_Sphere.h"

Tokutei_Sphere::Tokutei_Sphere()
{
}

Tokutei_Sphere::~Tokutei_Sphere()
{
}

void Tokutei_Sphere::Draw()
{
	pSphereMesh->Draw(transform);
}

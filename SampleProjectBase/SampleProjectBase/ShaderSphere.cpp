#include "pch.h"
#include "ShaderSphere.h"
#include "Geometory.h"

ShaderSphere::ShaderSphere()
{
	pSphereMesh = std::make_unique<Sphere>();
}

void ShaderSphere::Draw()
{
	pSphereMesh->Draw(transform);
}

void ShaderSphere::SetMaterial(MaterialClass& _material)
{
	pSphereMesh->SetMaterial(_material);
}
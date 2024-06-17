#include "pch.h"
#include "Material.h"
#include "ShaderCollection.h"

using namespace DirectX::SimpleMath;
Material::Material() : diffuse(Vector4::One), ambient(Vector4::One), specular(Vector4::One), 
					emissive(Vector4::One)
{
	ShaderCollection* shCol =  ShaderCollection::GetInstance();
	pVertexShader = shCol->GetVertexShader(shCol->defaultVS);
	pPixelShader = shCol->GetPixelShader(shCol->defaultPS);
}

Material::~Material()
{
}

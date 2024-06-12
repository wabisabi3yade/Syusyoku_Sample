#include "pch.h"
#include "Material.h"

using namespace DirectX::SimpleMath;
Material::Material() : diffuse(Vector4::One), ambient(Vector4::One), specular(Vector4::One), 
					emissive(Vector4::One), texture(nullptr), pVertexShader(nullptr),
					pPixelShader(nullptr)
{
}

Material::~Material()
{
}

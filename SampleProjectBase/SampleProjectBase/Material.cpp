#include "pch.h"
#include "Material.h"

Material::Material() : texture(nullptr)
{
}

Material::~Material()
{
	CLASS_DELETE(texture);
}

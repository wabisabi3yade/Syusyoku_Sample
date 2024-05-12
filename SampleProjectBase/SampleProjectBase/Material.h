#pragma once
#include "Texture.h"

// モデルの表面データ
class Material
{
public:
	Material();
	~Material();

	Float4 diffuse;
	Float4 ambient;
	Float4 specular;
	Texture* texture;	// テクスチャ
};


#pragma once
#include "Texture.h"

// モデルの表面データ
class Material
{
public:
	Material();
	~Material();

	DirectX::SimpleMath::Vector4 diffuse;
	DirectX::SimpleMath::Vector4 ambient;
	DirectX::SimpleMath::Vector4 specular;
	const Texture* texture;	// テクスチャ
};


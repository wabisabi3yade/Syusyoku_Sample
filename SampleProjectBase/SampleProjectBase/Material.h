#pragma once
#include "Texture.h"

class VertexShader;
class PixelShader;
// モデルの表面データ
class Material
{
public:
	Material();
	~Material();

	DirectX::SimpleMath::Color diffuse;	// 色
	DirectX::SimpleMath::Vector4 ambient;	// 環境光
	DirectX::SimpleMath::Vector4 specular;	// 鏡面反射
	DirectX::SimpleMath::Vector4 emissive;	// 自発光

	const Texture* texture;	// テクスチャ
	VertexShader* pVertexShader;
	PixelShader* pPixelShader;
};


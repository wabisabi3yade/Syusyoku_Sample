#pragma once
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"


struct MaterialParameter
{
	DirectX::SimpleMath::Color diffuse;	// 色
	DirectX::SimpleMath::Color ambient;	// 環境光
	DirectX::SimpleMath::Color specular;	// 鏡面反射
	DirectX::SimpleMath::Color emissive;	// 自発光
	float shininess;	// 光沢
};

// モデルの表面データ
class MaterialClass
{
	MaterialParameter parameter;	// マテリアルのパラメータ

	// 使用する各シェーダー
	VertexShader* pVertexShader;	// 頂点	
	PixelShader* pPixelShader;	// ピクセル
public:
	MaterialClass();
	~MaterialClass();

	// シェーダーを設定
	void SetVertexShader(VertexShader* _vs) { pVertexShader = _vs; }
	void SetPixelShader(PixelShader* _ps) { pPixelShader = _ps; }
	
	// マテリアル・各パラメータを設定
	void SetMaterialParameter(const MaterialParameter& _mat) { parameter = _mat; }
	void SetDiffuse(const DirectX::SimpleMath::Color& _diffuse) { parameter.diffuse = _diffuse; }
	void SetAmbeint(const DirectX::SimpleMath::Color& _ambient) { parameter.ambient = _ambient; }
	void SetSpecular(const DirectX::SimpleMath::Color& _specular) { parameter.specular = _specular; }
	void SetEmissive(const DirectX::SimpleMath::Color& _emmisive) { parameter.emissive = _emmisive; }

	VertexShader& GetVertexShader()const { return *pVertexShader; }
	PixelShader& GetPixelShader()const { return *pPixelShader; }
	MaterialParameter& GetMaterialParameter() { return parameter; }
};


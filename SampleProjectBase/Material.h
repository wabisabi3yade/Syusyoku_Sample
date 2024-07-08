#pragma once
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"


struct MaterialParameter
{
	// 色
	DirectX::SimpleMath::Color diffuse;	

	// 環境光
	DirectX::SimpleMath::Color ambient;

	// 鏡面反射
	DirectX::SimpleMath::Color specular;	

	// 自発光
	DirectX::SimpleMath::Color emissive;	

	// 光沢
	float shininess;

	// テクスチャ使用フラグ
	int isTextureEnable;	
};

// モデルの表面データ
class Material
{
	// マテリアルのパラメータ
	MaterialParameter parameter;	

	// 色テクスチャ
	Texture* pDiffuseTexture;	
	// 法線テクスチャ
	Texture* pNormalTexture;

	// 使用する各シェーダー
	// 頂点	
	VertexShader* pVertexShader;
	// ピクセル
	PixelShader* pPixelShader;	
public:
	Material();
	~Material();

	// シェーダーを設定
	void SetVertexShader(VertexShader* _vs) { pVertexShader = _vs; }
	void SetPixelShader(PixelShader* _ps) { pPixelShader = _ps; }
	
	// マテリアル・各パラメータをセット
	void SetMaterialParameter(const MaterialParameter& _mat) { parameter = _mat; }
	void SetDiffuse(const DirectX::SimpleMath::Color& _diffuse) { parameter.diffuse = _diffuse; }
	void SetAmbeint(const DirectX::SimpleMath::Color& _ambient) { parameter.ambient = _ambient; }
	void SetSpecular(const DirectX::SimpleMath::Color& _specular) { parameter.specular = _specular; }
	void SetEmissive(const DirectX::SimpleMath::Color& _emmisive) { parameter.emissive = _emmisive; }
	void SetTextureEnable(bool _isTextureEnable) { parameter.isTextureEnable = _isTextureEnable; }

	// 各テクスチャをセット
	void SetDiffuseTexture(Texture& _diffuseTex);
	void SetNormalTexture(Texture& _normalTex) { pNormalTexture = &_normalTex; }

	VertexShader& GetVertexShader()const { return *pVertexShader; }
	PixelShader& GetPixelShader()const { return *pPixelShader; }

	MaterialParameter& GetMaterialParameter() { return parameter; }

	Texture& GetDiffuseTexture()const { return *pDiffuseTexture; }
	Texture& GetNormalTexture()const { return *pNormalTexture; }

	void ImGuiSetting();
};


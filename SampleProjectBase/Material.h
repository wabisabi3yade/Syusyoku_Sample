#pragma once
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"

struct MaterialParameter
{
	/// @brief 色
	DirectX::SimpleMath::Color diffuse;	

	/// @brief 環境光
	DirectX::SimpleMath::Color ambient;

	/// @brief 鏡面反射
	DirectX::SimpleMath::Color specular;	

	/// @brief 自発光
	DirectX::SimpleMath::Color emissive;	

	/// @brief 光沢
	float shiness;

	/// @brief テクスチャ使用フラグ
	int isTextureEnable;	
};

// モデルの表面データ
class Material : public Asset_Base
{
	/// @brief パラメータ
	MaterialParameter parameter;	

	/// @brief テクスチャ
	Texture* pDiffuseTexture;	

	/// @brief 法線テクスチャ
	Texture* pNormalTexture;

	/// @brief 頂点シェーダー
	VertexShader* pVertexShader;
	
	/// @brief ピクセルシェーダー
	PixelShader* pPixelShader;	

	/// @brief 名前
	std::string name;
public:
	Material();
	~Material();

	/// @brief 頂点シェーダーセット
	/// @param _vs 頂点シェーダー
	void SetVertexShader(VertexShader* _vs) { pVertexShader = _vs; }

	/// @brief  ピクセルシェーダーセット
	/// @param _ps ピクセルシェーダー
	void SetPixelShader(PixelShader* _ps) { pPixelShader = _ps; }
	
	// マテリアル・各パラメータをセット
	void SetMaterialParameter(const MaterialParameter& _mat) { parameter = _mat; }
	void SetDiffuse(const DirectX::SimpleMath::Color& _diffuse) { parameter.diffuse = _diffuse; }
	void SetAmbeint(const DirectX::SimpleMath::Color& _ambient) { parameter.ambient = _ambient; }
	void SetSpecular(const DirectX::SimpleMath::Color& _specular) { parameter.specular = _specular; }
	void SetEmissive(const DirectX::SimpleMath::Color& _emmisive) { parameter.emissive = _emmisive; }
	void SetShiness(float _shiness) { parameter.shiness = _shiness; }
	void SetTextureEnable(bool _isTextureEnable) { parameter.isTextureEnable = _isTextureEnable; }

	// 各テクスチャをセット
	void SetDiffuseTexture(Texture& _diffuseTex);
	void SetNormalTexture(Texture& _normalTex) { pNormalTexture = &_normalTex; }

	/// @brief 名前をセット
	/// @param _name セットする名前
	void SetName(const std::string& _name) { name = _name; }

	VertexShader& GetVertexShader()const { return *pVertexShader; }
	PixelShader& GetPixelShader()const { return *pPixelShader; }

	MaterialParameter& GetMaterialParameter() { return parameter; }

	Texture& GetDiffuseTexture()const { return *pDiffuseTexture; }
	Texture& GetNormalTexture()const { return *pNormalTexture; }

	void ImGuiSetting();
};


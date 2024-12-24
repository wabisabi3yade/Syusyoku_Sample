#pragma once
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace HashiTaku
{
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

		float dummy1, dummy2;
	};

	// モデルの表面データ
	class Material : public Asset_Base, public IImGuiUser
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
	public:
		Material();
		Material(const std::string& _vsName, const std::string& _psName);
		~Material();

		/// @brief 描画準備
		/// @param _wvp wvp行列
		void DrawSetup(RenderParam::WVP& _wvp);

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

		// 頂点・ピクセルシェーダーをセット
		void SetVertexShader(const std::string& _vsName);
		void SetVertexShader(VertexShader& _vsShader);
		void SetPixelShader(const std::string& _psName);
		void SetPixelShader(PixelShader& _psShader);

		VertexShader& GetVertexShader()const { return *pVertexShader; }
		PixelShader& GetPixelShader()const { return *pPixelShader; }

		MaterialParameter& GetMaterialParameter() { return parameter; }

		Texture* GetDiffuseTexture()const { return pDiffuseTexture; }
		Texture* GetNormalTexture()const { return pNormalTexture; }

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief マテリアルのパラメータ初期化
		void InitParameter();

		/// @brief シェーダーの準備
		void ShaderSetup(Shader& _shader, RenderParam::WVP& _wvp);

		void ImGuiDebug() override;
	};
}
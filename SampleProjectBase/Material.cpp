#include "pch.h"
#include "Material.h"
#include "ShaderCollection.h"

#include "AssetGetter.h"

using namespace DirectX::SimpleMath;

constexpr u_int DIFFUSETEX_SLOT(0);

Material::Material() : pDiffuseTexture(nullptr), pNormalTexture(nullptr), pVertexShader(nullptr),
pPixelShader(nullptr)
{
	// デフォルトのシェーダーを初期設定しておく
	ShaderCollection* shCol = ShaderCollection::GetInstance();
	pVertexShader = shCol->GetVertexShader(shCol->defaultVS);
	pPixelShader = shCol->GetPixelShader(shCol->defaultPS);

	InitParameter();
}

Material::Material(const std::string& _vsName, const std::string& _psName)
{
	InitParameter();

	// デフォルトのシェーダーを初期設定しておく
	ShaderCollection* shCol = ShaderCollection::GetInstance();
	pVertexShader = shCol->GetVertexShader(_vsName);
	if (pVertexShader == nullptr)
		HASHI_DEBUG_LOG(_vsName + "：頂点シェーダーがありません");

	pPixelShader = shCol->GetPixelShader(_psName);
	if (pPixelShader == nullptr)
		HASHI_DEBUG_LOG(_psName + "：ピクセルシェーダーはありません");
}

Material::~Material()
{
}

void Material::DrawSetup(RenderParam::WVP& _wvp)
{
	// wvpとマテリアルのバッファ更新
	ShaderSetup(*pVertexShader, _wvp);
	ShaderSetup(*pPixelShader, _wvp);

	pPixelShader->SetTexture(DIFFUSETEX_SLOT, pDiffuseTexture);
}

void Material::SetDiffuseTexture(Texture& _diffuseTex)
{
	pDiffuseTexture = &_diffuseTex;
	parameter.isTextureEnable = true;
}

void Material::SetVertexShader(const std::string& _vsName)
{
	VertexShader* pVS = ShaderCollection::GetInstance()->GetVertexShader(_vsName);

	if (pVS == nullptr)
		return;

	pVertexShader = pVS;
}

void Material::SetVertexShader(VertexShader& _vsShader)
{
	pVertexShader = &_vsShader;
}

void Material::SetPixelShader(const std::string& _psName)
{
	PixelShader* pPS = ShaderCollection::GetInstance()->GetPixelShader(_psName);

	if (pPS == nullptr)
		return;

	pPixelShader = pPS;
}

void Material::SetPixelShader(PixelShader& _psShader)
{
	pPixelShader = &_psShader;
}

void Material::ImGuiSetting()
{
	// テクスチャ
	std::string texName;
	if (pDiffuseTexture)
		texName = pDiffuseTexture->GetAssetName();
	if (AssetGetter::ImGuiGetCombobox<Texture>("Diffuse", texName))
	{
		pDiffuseTexture = AssetGetter::GetAsset<Texture>(texName);
	}

	ImGuiMethod::ColorEdit4(parameter.diffuse, "diffuse");
	ImGuiMethod::ColorEdit4(parameter.ambient, "ambient");
	ImGuiMethod::ColorEdit4(parameter.specular, "specular");
	ImGuiMethod::ColorEdit4(parameter.emissive, "emmisive");
	ImGui::DragFloat("shiness", &parameter.shiness);
}

nlohmann::json Material::Save()
{
	using namespace HashiTaku;

	auto data = Asset_Base::Save();

	SaveJsonVector4("m_diffuse", parameter.diffuse, data);
	SaveJsonVector4("m_ambient", parameter.ambient, data);
	SaveJsonVector4("m_specular", parameter.specular, data);
	SaveJsonVector4("m_emissive", parameter.emissive, data);
	data["m_shiness"] = parameter.shiness;

	if (pDiffuseTexture)
		data["diffuseTex"] = pDiffuseTexture->GetAssetName();
	if (pNormalTexture)
		data["normalTex"] = pNormalTexture->GetAssetName();
	if (pVertexShader)
		data["vertexShader"] = pVertexShader->GetShaderName();
	if (pPixelShader)
		data["pixelShader"] = pPixelShader->GetShaderName();

	return data;
}

void Material::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;

	Asset_Base::Load(_data);

	LoadJsonColor("m_diffuse", parameter.diffuse, _data);
	LoadJsonColor("m_ambient", parameter.ambient, _data);
	LoadJsonColor("m_specular", parameter.specular, _data);
	LoadJsonColor("m_emissive", parameter.emissive, _data);
	LoadJsonFloat("m_shiness", parameter.shiness, _data);

	pDiffuseTexture = LoadJsonAsset<Texture>("diffuseTex", _data);
	pNormalTexture = LoadJsonAsset<Texture>("normalTex", _data);

	ShaderCollection* shCol = ShaderCollection::GetInstance();
	std::string shaderName = "";
	LoadJsonString("vertexShader", shaderName, _data);
	VertexShader* pLoadVS = shCol->GetVertexShader(shaderName);
	if (pLoadVS)
		pVertexShader = pLoadVS;

	LoadJsonString("pixelShader", shaderName, _data);
	PixelShader* pLoadPs = shCol->GetPixelShader(shaderName);
	if (pLoadPs)
		pPixelShader = pLoadPs;

	if (pDiffuseTexture)	// テクスチャあるなら
		parameter.isTextureEnable = 1;
}

void Material::InitParameter()
{
	// 初期化
	parameter.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	parameter.ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
	parameter.specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
	parameter.emissive = Color(1.0f, 1.0f, 1.0f, 1.0f);
	parameter.shiness = 0.0f;
	parameter.isTextureEnable = FALSE;
}

void Material::ShaderSetup(Shader& _shader, RenderParam::WVP& _wvp)
{
	// WVP・マテリアルのバッファを送る
	u_int bufferNum = _shader.GetBufferNum();

	using enum Shader::BufferType;
	for (u_int bufLoop = 0; bufLoop < bufferNum; bufLoop++)
	{
		switch (_shader.GetBufferType(bufLoop))
		{
		case WVP:
			_shader.UpdateSubResource(bufLoop, &_wvp);
			break;

		case Material:
			_shader.UpdateSubResource(bufLoop, &parameter);
			break;

		default:
			break;
		}
	}
}
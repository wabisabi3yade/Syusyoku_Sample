#include "pch.h"
#include "Material.h"
#include "ShaderCollection.h"

// ImGui
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

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

void Material::ImGuiSetting()
{
#ifdef EDIT
	ImGuiMethod::ColorEdit4(parameter.diffuse, "diffuse");
	ImGuiMethod::ColorEdit4(parameter.ambient, "ambient");
	ImGuiMethod::ColorEdit4(parameter.specular, "specular");
	ImGuiMethod::ColorEdit4(parameter.emissive, "emmisive");
	ImGui::DragFloat("shiness", &parameter.shiness);
#endif
}

void Material::InitParameter()
{
	// 初期化
	parameter.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	parameter.ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
	parameter.specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
	parameter.emissive = Color(1.0f, 1.0f, 1.0f, 1.0f);
	parameter.shiness = 0.0f;
	parameter.isTextureEnable = 0;
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
			_shader.UpdateBuffer(bufLoop, &_wvp);
			break;

		case Material:
			_shader.UpdateBuffer(bufLoop, &parameter);
			break;

		default:
			break;
		}
	}
}
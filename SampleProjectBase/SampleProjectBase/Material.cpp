#include "pch.h"
#include "Material.h"
#include "ShaderCollection.h"

// ImGui
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"


using namespace DirectX::SimpleMath;
Material::Material()
{
	// デフォルトのシェーダーを初期設定しておく
	ShaderCollection* shCol =  ShaderCollection::GetInstance();
	pVertexShader = shCol->GetVertexShader(shCol->defaultVS);
	pPixelShader = shCol->GetPixelShader(shCol->defaultPS);

	// 初期化
	parameter.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	parameter.ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
	parameter.specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
	parameter.emissive = Color(1.0f, 1.0f, 1.0f, 1.0f);
	parameter.shininess = 0.0f;
	parameter.isTextureEnable = 0;
}

Material::~Material()
{
}

void Material::ImGuiSetting()
{
#ifdef EDIT
		ImGuiMethod::ColorEdit4(parameter.diffuse, "diffuse");
		ImGuiMethod::ColorEdit4(parameter.ambient, "ambient");
		ImGuiMethod::ColorEdit4(parameter.specular, "specular");
		ImGuiMethod::ColorEdit4(parameter.emissive, "emmisive");
		ImGui::DragFloat("shiness", &parameter.shininess);
#endif
}


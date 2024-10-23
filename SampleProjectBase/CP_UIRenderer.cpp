#include "pch.h"
#include "CP_UIRenderer.h"
#include "ShaderCollection.h"
#include "GameObject.h"
#include "AssetGetter.h"

VertexShader* CP_UIRenderer::pVertexShader = nullptr;
PixelShader* CP_UIRenderer::pPixelShader = nullptr;
bool CP_UIRenderer::isSetShader = false;

constexpr auto VS_NAME("VS_UI");	// 頂点シェーダー名
constexpr auto PS_NAME("PS_UI");	// ピクセルシェーダー名

CP_UIRenderer::CP_UIRenderer() : pTexture(nullptr)
{
}

void CP_UIRenderer::Init()
{
	CP_Renderer::Init();

	// 準備されていないなら
	if (!isSetShader)
	{
		// シェーダーを取得する
		ShaderCollection& shaderCol = *ShaderCollection::GetInstance();
		pVertexShader = shaderCol.GetVertexShader(VS_NAME);
		pPixelShader = shaderCol.GetPixelShader(PS_NAME);

		isSetShader = true;
	}

	// 2Dポリゴン作成
	pPolygon = std::make_unique<Polygon2D>();

	// UIに変更する
	gameObject->SetLayer(Layer::Type::UI);
}

void CP_UIRenderer::OnChangeScale()
{
	ReCreatePolygon();
}

void CP_UIRenderer::OnChangePosition()
{
	ReCreatePolygon();
}

void CP_UIRenderer::OnChangeRotation()
{
	ReCreatePolygon();
}

void CP_UIRenderer::SetTexture(const Texture& _setTexture)
{
	pTexture = &_setTexture;
}

nlohmann::json CP_UIRenderer::Save()
{
	return Component::Save();
}

void CP_UIRenderer::Load(const nlohmann::json& _data)
{
	ReCreatePolygon();
}

void CP_UIRenderer::Draw()
{
	// 準備
	DrawSetup();

	// ポリゴン描画
	DrawMesh(*pPolygon);
}

void CP_UIRenderer::DrawSetup()
{
	using namespace DirectX::SimpleMath;

	// テクスチャを使用するかシェーダーに渡す構造体
	struct TexEnable
	{
		int isTexEnable = 0;
		int dummy[3];
	};

	// レンダラー取得
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// シェーダーの設定
	TexEnable texEnable;
	texEnable.isTexEnable = pTexture != nullptr;

	// 投影
	Matrix projection = renderer.GetParameter().GetProjectionMatrix();

	pVertexShader->UpdateSubResource(0, &projection);
	pPixelShader->UpdateSubResource(0, &texEnable);
	pPixelShader->SetTexture(0, pTexture);
	pVertexShader->SetGPU();
	pPixelShader->SetGPU();
}

void CP_UIRenderer::ReCreatePolygon()
{
	using namespace DirectX::SimpleMath;
	const Transform& t = GetTransform();

	Vector2 p = Vector2(t.GetPosition().x, t.GetPosition().y);
	Vector2 s = Vector2(t.GetScale().x, t.GetScale().y);
	pPolygon->MakePolygon(p, s, t.GetEularAngles());
}

void CP_UIRenderer::ImGuiSetting()
{
	std::string texName;
	if (pTexture)
		texName = pTexture->GetAssetName();

	if (AssetGetter::ImGuiGetCombobox<Texture>("Image", texName))
	{
		pTexture = AssetGetter::GetAsset<Texture>(texName);
	}
}

#include "pch.h"
#include "CP_SpriteRenderer.h"
#include "ShaderCollection.h"

#include "Material.h"

#include "InSceneSystemManager.h"

void CP_SpriteRenderer::Copy(const CP_SpriteRenderer& _other)
{
}

void CP_SpriteRenderer::DrawSetup()
{
	// レンダラー取得
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// ワールド変換行列を取得
	DirectX::SimpleMath::Matrix worldMatrix = D3D11_Renderer::GetWorldMtx(GetTransform());

	// ワールド変換行列の座標にモデルの座標を入れる
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP();
	wvp.world = worldMatrix;

	// シェーダーの設定
	VertexShader& pVs = pMaterial->GetVertexShader();
	PixelShader& pPs = pMaterial->GetPixelShader();

	pVs.UpdateBuffer(0, &wvp);
	MaterialParameter materialParam = pMaterial->GetMaterialParameter();
	pVs.UpdateBuffer(1, &materialParam);

	// ディレクションライトの情報を取得
	SceneLights& sceneLights = InSceneSystemManager::GetInstance()->GetSceneLights();
	DirectionLParameter dirLightParam = sceneLights.GetDirectionParameter();
	pVs.UpdateBuffer(2, &dirLightParam);

	Texture& diffuseTex = pMaterial->GetDiffuseTexture();
	pPs.UpdateBuffer(0, &materialParam);
	pPs.SetTexture(0, &diffuseTex);

	pVs.Bind();
	pPs.Bind();
}

void CP_SpriteRenderer::MaterialSetup()
{
	// デフォルトでマテリアルを設定する
	const std::string MATERIAL_NAME = "M_SpriteUnlit";
	ResourceCollection* recol = ResourceCollection::GetInstance();
	if (recol->GetImpotred(MATERIAL_NAME))	// デフォルトのマテリアルがある
	{
		pMaterial = recol->GetResource<Material>(MATERIAL_NAME);
	}
	else		// ない
	{
		// マテリアルを作成し、管理クラスにセット
		std::unique_ptr<Material> createMaterial = std::make_unique<Material>();

		ShaderCollection* shCol = ShaderCollection::GetInstance();
		VertexShader* pVs = shCol->GetVertexShader(shCol->defaultVS);
		PixelShader* pPs = shCol->GetPixelShader(shCol->defaultPS);
		createMaterial->SetVertexShader(pVs);
		createMaterial->SetPixelShader(pPs);

		pMaterial = ResourceCollection::GetInstance()->SetResource<Material>(MATERIAL_NAME, std::move(createMaterial));
	}
}

CP_SpriteRenderer& CP_SpriteRenderer::operator=(const CP_SpriteRenderer& _other)
{
	if (this == &_other) return *this;

	Copy(_other);

	return *this;
}

void CP_SpriteRenderer::Init()
{
	name = "SpriteRenderer";

	// スプライト作成
	pSprite = std::make_unique<Sprite>();

	// マテリアル初期化
	MaterialSetup();
}

void CP_SpriteRenderer::Draw()
{
	// 描画準備
	DrawSetup();

	pSprite->Draw();
}

void CP_SpriteRenderer::SetTexture(Texture& _texture)
{
	// マテリアルに渡す
	pMaterial->SetDiffuseTexture(_texture);

	// スプライトに渡す
	pSprite->SetTexture(_texture);
}

void CP_SpriteRenderer::SetMaterial(Material& _material)
{
	pMaterial = &_material;
}

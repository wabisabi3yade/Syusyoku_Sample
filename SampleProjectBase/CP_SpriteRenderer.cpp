#include "pch.h"
#include "CP_SpriteRenderer.h"
#include "ShaderCollection.h"

// アセット関連
#include "AssetSetter.h"
#include "AssetGetter.h"

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

	Texture& pTex = pSprite->GetTexture();
	pPs.UpdateBuffer(0, &materialParam);
	pPs.SetTexture(0, &pTex);

	pVs.Bind();
	pPs.Bind();
}

void CP_SpriteRenderer::MaterialSetup()
{
	// デフォルトでマテリアルを設定する
	const std::string MATERIAL_NAME = "M_SpriteUnlit";
	
	// 既に作成済みなら
	if (AssetSetter::CheckImport<Material>(MATERIAL_NAME))
	{
		pMaterial = AssetGetter::GetAsset<Material>(MATERIAL_NAME);
		return;
	}

	// マテリアル作成し、アセットをセットする
	std::unique_ptr<Material> pCreateMaterial = std::make_unique<Material>();
	pMaterial = AssetSetter::SetAsset(MATERIAL_NAME, std::move(pCreateMaterial));
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
}

void CP_SpriteRenderer::SetTexture(Texture& _texture)
{
	// スプライトに渡す
	pSprite->SetTexture(_texture);
}

void CP_SpriteRenderer::SetMaterial(Material& _material)
{
	pMaterial = &_material;
}

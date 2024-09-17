#include "pch.h"
#include "CP_SpriteRenderer.h"
#include "ShaderCollection.h"

// アセット関連
#include "AssetSetter.h"
#include "AssetGetter.h"

#include "InSceneSystemManager.h"

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


void CP_SpriteRenderer::DrawSetup()
{
	struct TexEnable
	{
		int isTexEnable;
		float dummy[3];
	};

	// レンダラー取得
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// ワールド変換行列の座標にモデルの座標を入れる
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP(GetTransform());
	wvp.world = wvp.world.Transpose();

	// シェーダーの設定
	VertexShader& pVs = pMaterial->GetVertexShader();
	PixelShader& pPs = pMaterial->GetPixelShader();

	MaterialParameter& materialParam = pMaterial->GetMaterialParameter();
	materialParam.isTextureEnable = pSprite->GetIsTexEnable();

	Texture* pTex = pSprite->GetTexture();

	pVs.UpdateSubResource(0, &wvp);
	pVs.UpdateSubResource(1, &materialParam);

	TexEnable texEnable;
	texEnable.isTexEnable = materialParam.isTextureEnable;
	pPs.UpdateSubResource(0, &texEnable);
	pPs.SetTexture(0, pTex);

	pVs.SetGPU();
	pPs.SetGPU();
}

CP_SpriteRenderer::CP_SpriteRenderer()
{

}

CP_SpriteRenderer::CP_SpriteRenderer(const CP_SpriteRenderer& _other)
{
	Copy(_other);
}

CP_SpriteRenderer& CP_SpriteRenderer::operator=(const CP_SpriteRenderer& _other)
{
	Copy(_other);

	return *this;
}

void CP_SpriteRenderer::Init()
{
	// スプライト作成
	pSprite = std::make_unique<Sprite>();

	// マテリアル初期化
	MaterialSetup();
}

void CP_SpriteRenderer::Draw()
{
	// 描画準備
	DrawSetup();

	// 四角形ポリゴンを描画
	CP_Renderer::DrawMesh(pSprite->GetSquare());
}

void CP_SpriteRenderer::ImGuiSetting()
{
	const u_int Buf = 256;
	static char str[Buf];
	ImGui::InputText("texture", str, Buf);

	if (ImGui::Button("Set"))
	{
		Texture* pTex = AssetGetter::GetAsset<Texture>(str);

		if (pTex)
			pSprite->SetTexture(*pTex);
	}
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

nlohmann::json CP_SpriteRenderer::Save()
{
	auto data = CP_Renderer::Save();

	data["sprite"] = pSprite->Save();

	return data;
}

void CP_SpriteRenderer::Load(const nlohmann::json& _data)
{
	CP_Renderer::Load(_data);

	if (!HashiTaku::IsJsonContains(_data, "sprite")) return;

	pSprite->Load(_data["sprite"]);
}

void CP_SpriteRenderer::Copy(const CP_SpriteRenderer& _other)
{
	if (this == &_other) return;

	CP_Renderer::operator=(_other);

	if (_other.pSprite)
		pSprite = std::make_unique<Sprite>(*_other.pSprite);

	if (_other.pMaterial)
		pMaterial = _other.pMaterial;
}

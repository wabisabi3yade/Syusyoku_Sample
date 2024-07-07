#include "pch.h"
#include "CP_MeshRenderer.h"

// システム
#include "InSceneSystemManager.h"
#include "ShaderCollection.h"

void CP_MeshRenderer::DrawSetup()
{
	// レンダラー取得
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
	// ワールド変換行列を取得
	DirectX::SimpleMath::Matrix worldMatrix = D3D11_Renderer::GetWorldMtx(GetTransform());

	// ワールド変換行列の座標にモデルの座標を入れる
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP();
	wvp.world = worldMatrix;

	// シェーダーの設定
	VertexShader& pVs = pMaterials->GetVertexShader();
	PixelShader& pPs = pMaterials->GetPixelShader();

	pVs.UpdateBuffer(0, &wvp);

	MaterialParameter& materialParam = pMaterials->GetMaterialParameter();
	pVs.UpdateBuffer(1, &materialParam);

	// ディレクションライトの情報を取得
	SceneLights& sceneLights = InSceneSystemManager::GetInstance()->GetSceneLights();
	DirectionLParameter dirLightParam = sceneLights.GetDirectionParameter();
	pVs.UpdateBuffer(2, &dirLightParam);

	pPs.UpdateBuffer(0, &materialParam);

	pVs.Bind();
	pPs.Bind();
}

void CP_MeshRenderer::Init()
{
	name = "Mesh_Renderer";

	// デフォルトでキューブを入れておく
	pMeshes = ResourceCollection::GetInstance()->GetResource<Mesh>("SM_Cube");

	ResourceCollection* reCol = ResourceCollection::GetInstance();
	// 既にあるか確認
	const std::string MATERIAL_NAME = "M_Unlit";

	if (!reCol->GetImpotred(MATERIAL_NAME))	// 無かったら
	{
		// マテリアル作成
		std::unique_ptr<Material> makeMaterial = std::make_unique<Material>();
		// シェーダーを設定
		ShaderCollection* shCol = ShaderCollection::GetInstance();
		VertexShader* v = shCol->GetVertexShader(shCol->defaultVS);
		PixelShader* p = shCol->GetPixelShader(shCol->defaultPS);
		makeMaterial->SetVertexShader(v);
		makeMaterial->SetPixelShader(p);

		// 管理クラスにセット
		pMaterials = reCol->SetResource(MATERIAL_NAME, std::move(makeMaterial));
	}
	else	// あるなら
	{
		// マテリアルを取得
		pMaterials = reCol->GetResource<Material>(MATERIAL_NAME);
	}
}

void CP_MeshRenderer::Draw()
{
	for (auto pMesh : pMeshes)
	{
		DrawSetup();

		pMesh->Draw();
	}	
}

void CP_MeshRenderer::ImGuiSetting()
{
	for()
	pMaterials->ImGuiSetting();
}

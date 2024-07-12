#include "pch.h"
#include "CP_MeshRenderer.h"

// システム
#include "InSceneSystemManager.h"
#include "ShaderCollection.h"

const RenderParam::WVP CP_MeshRenderer::WVPSetup()
{
	// レンダラー取得
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
	// ワールド変換行列を取得
	DirectX::SimpleMath::Matrix worldMatrix = D3D11_Renderer::GetWorldMtx(GetTransform());

	// ワールド変換行列の座標にモデルの座標を入れる
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP();
	wvp.world = worldMatrix;

	return wvp;
}

//void CP_MeshRenderer::MeshDraw(u_int _meshIdx, RenderParam::WVP& _wvp)
//{
//	//RenderMesh& renderMesh = renderMeshes[_meshIdx];
//
//	//// シェーダーの設定
//	//Material& useMaterial = *pMaterials[renderMesh.materialID];
//	//VertexShader& pVs = useMaterial.GetVertexShader();
//	//PixelShader& pPs = useMaterial.GetPixelShader();
//
//	//pVs.UpdateBuffer(0, &_wvp);
//
//	//MaterialParameter& materialParam = useMaterial.GetMaterialParameter();
//	//pVs.UpdateBuffer(1, &materialParam);
//
//	//// ディレクションライトの情報を取得
//	//SceneLights& sceneLights = InSceneSystemManager::GetInstance()->GetSceneLights();
//	//DirectionLParameter dirLightParam = sceneLights.GetDirectionParameter();
//	//pVs.UpdateBuffer(2, &dirLightParam);
//
//	//pPs.UpdateBuffer(0, &materialParam);
//
//	//pVs.Bind();
//	//pPs.Bind();
//
//	//// 描画
//	//renderMesh.pMesh->Draw();
//}

void CP_MeshRenderer::Init()
{
	//name = "Mesh_Renderer";

	//// デフォルトのメッシュを入れておく
	//AssetCollection& resourceCollect = *AssetCollection::GetInstance();
	//RenderMesh renderMesh;
	//renderMesh.pMesh = resourceCollect.GetResource<Mesh>("SM_Cube");
	//renderMesh.materialID = 0;
	//renderMeshes.push_back(renderMesh);

	//// マテリアルがあるか確認
	//const std::string MATERIAL_NAME = "M_Unlit";
	//if (!resourceCollect.GetImpotred(MATERIAL_NAME))	// 無かったら
	//{
	//	// マテリアル作成
	//	std::unique_ptr<Material> makeMaterial = std::make_unique<Material>();

	//	// シェーダーをセット
	//	ShaderCollection* shCol = ShaderCollection::GetInstance();
	//	VertexShader* defaultVS = shCol->GetVertexShader(shCol->defaultVS);
	//	PixelShader* defaultPS = shCol->GetPixelShader(shCol->defaultPS);
	//	makeMaterial->SetVertexShader(defaultVS);
	//	makeMaterial->SetPixelShader(defaultPS);

	//	// 管理クラスにセット
	//	resourceCollect.SetResource(MATERIAL_NAME, std::move(makeMaterial));
	//}

	//// マテリアル
	//Material& defaultMaterial = *resourceCollect.GetResource<Material>(MATERIAL_NAME);
	//pMaterials.push_back(&defaultMaterial);
}

void CP_MeshRenderer::Draw()
{
	//// ワールド変換行列を求める
	//RenderParam::WVP wvp = WVPSetup();

	//for (u_int meshIdx = 0; meshIdx < static_cast<u_int>(renderMeshes.size()); meshIdx++)
	//{
	//	assert(renderMeshes[meshIdx].pMesh != nullptr && "メッシュ、マテリアルがない");

	//	// メッシュ描画
	//	MeshDraw(meshIdx, wvp);
	//}
}
//
//void CP_MeshRenderer::SetModel(Model& _model)
//{
//
//}

void CP_MeshRenderer::SetRenderMesh(Mesh_Base& _renderMesh)
{
	pRenderMesh = &_renderMesh;
}

void CP_MeshRenderer::ImGuiSetting()
{

}

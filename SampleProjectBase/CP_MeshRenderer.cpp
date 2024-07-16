#include "pch.h"
#include "CP_MeshRenderer.h"

// システム
#include "AssetSetter.h"
#include "AssetGetter.h"
#include "InSceneSystemManager.h"
#include "ShaderCollection.h"

// メッシュ
#include "StaticMesh.h"
#include "SkeletalMesh.h"



void CP_MeshRenderer::Init()
{
	name = "Mesh_Renderer";

	// デフォルトのメッシュを入れておく
	pRenderMesh = AssetGetter::GetAsset<StaticMesh>("SM_Cube");
}

void CP_MeshRenderer::Draw()
{
	RenderParam& rendererParam = Direct3D11::GetInstance()->GetRenderer()->GetParameter();

	// メッシュ描画
	DrawMesh(rendererParam.GetWVP(GetTransform()));
}

void CP_MeshRenderer::SetRenderMesh(Mesh_Base& _renderMesh)
{
	pRenderMesh = &_renderMesh;
}

void CP_MeshRenderer::ImGuiSetting()
{
}

const Mesh_Base* CP_MeshRenderer::GetRenderMesh()
{
	return pRenderMesh;
}

void CP_MeshRenderer::DrawMesh(RenderParam::WVP _wvp)
{
	u_int meshNum = pRenderMesh->GetMeshNum();
	for (u_int meshLoop = 0; meshLoop < meshNum; meshLoop++)
	{
		// メッシュを取得
		const SingleMesh* pSingleMesh = pRenderMesh->GetMesh(meshLoop);

		// 使用するマテリアル取得
		u_int materialID = pSingleMesh->GetMaterialID();
		Material* pMaterial = pRenderMesh->GetMaterial(materialID);

		// シェーダーの設定
		VertexShader& pVs = pMaterial->GetVertexShader();
		PixelShader& pPs = pMaterial->GetPixelShader();

		pVs.UpdateBuffer(0, &_wvp);
		MaterialParameter& materialParam = pMaterial->GetMaterialParameter();

		pVs.UpdateBuffer(1, &materialParam);

		// ディレクションライトの情報を取得
		SceneLights& sceneLights = InSceneSystemManager::GetInstance()->GetSceneLights();
		DirectionLParameter dirLightParam = sceneLights.GetDirectionParameter();
		pVs.UpdateBuffer(2, &dirLightParam);

		pPs.UpdateBuffer(0, &materialParam);


		if(materialParam.isTextureEnable)
		pPs.SetTexture(0, &pMaterial->GetDiffuseTexture());

		pVs.Bind();
		pPs.Bind();

		CP_Renderer::DrawMesh(*pSingleMesh);
	}
}
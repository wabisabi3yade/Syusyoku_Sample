#include "pch.h"
#include "CP_OutLineRenderer.h"
#include "ShaderCollection.h"
#include "GameObject.h"

namespace HashiTaku
{
	VertexShader* CP_OutLineRenderer::pOutLineVS = nullptr;
	PixelShader* CP_OutLineRenderer::pOutLinePS = nullptr;

	constexpr auto VS_NAME = "VS_OutLine";
	constexpr auto ANIMVS_NAME = "VS_OutLine";
	constexpr auto PS_NAME = "PS_OutLine";

	CP_OutLineRenderer::CP_OutLineRenderer()
		: pRenderMesh(nullptr)
	{
	}

	void CP_OutLineRenderer::Init()
	{
		GetShader();
	}

	json CP_OutLineRenderer::Save()
	{
		auto data = CP_Renderer::Save();

		SaveJsonVector4("color", lineParameter.lineColor, data);
		data["scale"] = lineParameter.lineScale;

		return data;
	}

	void CP_OutLineRenderer::Load(const json& _data)
	{
		CP_Renderer::Load(_data);

		LoadJsonColor("color", lineParameter.lineColor, _data);
		LoadJsonFloat("scale", lineParameter.lineScale, _data);
	}

	void CP_OutLineRenderer::Start()
	{
		GetRenderMesh();
	}

	void CP_OutLineRenderer::Draw()
	{
#ifdef EDIT
		// 描画するものがないなら
		if (!pRenderMesh) return;
#endif // EDIT

		// 描画準備
		DrawSetup();

		// 描画する
		u_int meshNum = pRenderMesh->GetMeshNum();
		for (u_int meshLoop = 0; meshLoop < meshNum; meshLoop++)
		{
			// メッシュを取得
			const SingleMesh* pSingleMesh = pRenderMesh->GetMesh(meshLoop);
			CP_Renderer::DrawMesh(*pSingleMesh);
		}

		auto pRenderer = Direct3D11::GetInstance()->GetRenderer();
		pRenderer->SetCullingMode(D3D11_CULL_BACK);
		pRenderer->SerDepthWrite(true);
	}

	void CP_OutLineRenderer::DrawSetup()
	{
		auto pRenderer = Direct3D11::GetInstance()->GetRenderer();
		RenderParam& rendererParam = pRenderer->GetParameter();

		pRenderer->SetCullingMode(D3D11_CULL_BACK);
		pRenderer->SerDepthWrite(false);

		// シェーダーにバッファを送る
		// (ここではライト、カメラ座標などの1ループで1度しか送らないものは送らない)
		Transform& transform = GetTransform();
		auto& wvp = rendererParam.GetWVP();

		// ワールド行列作成
		wvp.world = MakeLoadMatrix() * transform.GetWorldMatrix();
		wvp.world = wvp.world.Transpose();

		// バッファーを送る
		pOutLineVS->UpdateSubResource(0, &wvp);
		pOutLineVS->UpdateSubResource(1, &lineParameter);

		// GPUに送る
		pOutLineVS->SetGPU();
		pOutLinePS->SetGPU();
	}

	void CP_OutLineRenderer::GetShader()
	{
		// シェーダーがまだないなら
		if (pOutLineVS) return;

		ShaderCollection* pShCol = ShaderCollection::GetInstance();
		pOutLineVS = pShCol->GetVertexShader(VS_NAME);
		pOutLinePS = pShCol->GetPixelShader(PS_NAME);
	}

	void CP_OutLineRenderer::GetRenderMesh()
	{
		// メッシュを取得する
		CP_MeshRenderer* pMeshRender = GetGameObject().GetComponent<CP_MeshRenderer>();
		if (!pMeshRender)	// ないなら
		{
			// 活動しないようにする
			SetEnable(false);
			return;
		}

		pRenderMesh = pMeshRender->GetRenderMesh();
		// メッシュレンダラーより描画を遅くする
		SetPriority(pMeshRender->GetPriority() + 1);
	}

	DXSimp::Matrix CP_OutLineRenderer::MakeLoadMatrix()
	{
		float loadScale = pRenderMesh->GetLoadOffsetScale();
		DXSimp::Vector3 loadAngles = pRenderMesh->GetLoadOffsetAngles() * Mathf::degToRad;

		return DXSimp::Matrix::CreateScale(DXSimp::Vector3::One * loadScale) *
			DXSimp::Matrix::CreateFromYawPitchRoll(loadAngles.y, loadAngles.x, loadAngles.z);
	}

	void CP_OutLineRenderer::ImGuiDebug()
	{
		ImGui::ColorEdit4("Color", &lineParameter.lineColor.x);
		ImGui::DragFloat("Scale", &lineParameter.lineScale, 0.001f, 0.0f, 100.0f);
	}
}
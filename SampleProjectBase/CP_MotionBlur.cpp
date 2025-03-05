#include "pch.h"
#include "CP_MotionBlur.h"
#include "GameObject.h"

namespace HashiTaku
{
	CP_MotionBlur::CP_MotionBlur() :
		pMeshRenderer(nullptr),
		pRTCollection(nullptr),
		pWriteVS(nullptr),
		pWritePS(nullptr),
		isDrawingBlur(false)
	{
	}

	void CP_MotionBlur::Init()
	{
		CP_Renderer::Init();

		// レンダーターゲット管理
		pRTCollection = &Direct3D11::GetInstance()->GetRenderer()->GetRTCollection();

		// 速度ベクトルをレンダーターゲットに書き込むシェーダー
		ShaderCollection& shCol = *ShaderCollection::GetInstance();
		pWriteVS = shCol.GetVertexShader("VS_DrawMotionVector");
		pWritePS = shCol.GetPixelShader("PS_DrawMotionVector");
	}

	void CP_MotionBlur::Start()
	{
		CP_Renderer::Start();

		// メッシュレンダラーを取得
		pMeshRenderer = GetGameObject().GetComponent<CP_MeshRenderer>();

		// 活動状態をオフ
		if (!pMeshRenderer)
		{
			SetEnable(false);
			return;
		}
		
		// メッシュレンダラーよりあとに更新
		SetPriority(pMeshRenderer->GetPriority() - 1);
	}

	void CP_MotionBlur::Draw()
	{
		// メッシュレンダーがないなら
#ifdef EDIT
		if (!pMeshRenderer) return;
#endif // EDIT

		CP_Renderer::Draw();

		// 現在フレームのメッシュ描画を行う
		const RenderParam::WVP& curWVP = pMeshRenderer->GetDrawMatrix();

		// ブラーを描画する
		if (isDrawingBlur)
		{
			DrawBlurTexture(curWVP);
		}

		// 次フレームの為に更新
		prevDrawMeshWVP = curWVP;
	}

	void CP_MotionBlur::SetDrawBlur(bool _drawBlur)
	{
		isDrawingBlur = _drawBlur;
	}

	void CP_MotionBlur::DrawSetup(const RenderParam::WVP& _curDrawMeshWVP)
	{
		// 速度ベクトルを書き込むレンダーターゲットに変更
		pRTCollection->
			SetRenderTarget(RenderTargetCollection::RenderTargetType::MotionVector, true);

		/// @brief テクスチャに速度ベクトルを書き込む
		struct WVPBuffer
		{
			/// @brief 1フレーム前の行列
			RenderParam::WVP prevDrawMeshWVP;

			/// @brief 今フレームの行列
			RenderParam::WVP curDrawMeshWVP;
		};

		// バッファを送る
		WVPBuffer buffer;
		buffer.prevDrawMeshWVP = prevDrawMeshWVP;
		buffer.curDrawMeshWVP = _curDrawMeshWVP;

		// バッファを更新
		pWriteVS->UpdateSubResource(0, &buffer);

		// シェーダーを設定
		pWriteVS->SetGPU();
		pWritePS->SetGPU();
	}

	void CP_MotionBlur::DrawBlurTexture(const RenderParam::WVP& _curDrawMeshWVP)
	{
		// 描画するメッシュを取得
		Mesh_Group* pRenderMesh = pMeshRenderer->GetRenderMesh();
#ifdef EDIT
		if (!pRenderMesh)
		{
			HASHI_DEBUG_LOG("描画にメッシュが設定されていません");
			return;
		}
#endif // EDIT
		
		// 描画準備
		DrawSetup(_curDrawMeshWVP);

		// 前フレームとの速度ベクトルをレンダーターゲットに描画
		u_int meshCnt = pRenderMesh->GetMeshNum();
		for (u_int m_i = 0; m_i < meshCnt; m_i++)
		{
			SingleMesh* pSingleMesh = pRenderMesh->GetMesh(m_i);
			DrawMesh(*pSingleMesh);
		}

		HASHI_DEBUG_LOG("ブラー描画中");
	}
	void CP_MotionBlur::ImGuiDebug()
	{
		CP_Renderer::ImGuiDebug();

		ImGui::Checkbox("UseBlur", &isDrawingBlur);
	}
}

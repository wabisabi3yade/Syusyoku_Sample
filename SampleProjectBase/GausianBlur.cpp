#include "pch.h"
#include "GausianBlur.h"

namespace HashiTaku
{
	constexpr auto VS_NAME("VS_FullScreenQuad");
	constexpr auto VERT_PS_NAME("PS_GausianVertBlur");
	constexpr auto HORI_PS_NAME("PS_GaussianHoriBlur");

	GausianBlur::GausianBlur(u_int _width, u_int _height)
	{
		pRenderer = Direct3D11::GetInstance()->GetRenderer();

		// レンダーターゲットを作成
		pBlurRenderTarget = std::make_unique<RenderTarget>();
		pBlurRenderTarget->Create(DXGI_FORMAT_R32G32B32A32_FLOAT,
			_width,
			_height);

		// シェーダーを取得
		ShaderCollection* pShCol = ShaderCollection::GetInstance();
		pGausianVS = pShCol->GetVertexShader(VS_NAME);
		pVertBlurPS = pShCol->GetPixelShader(VERT_PS_NAME);
		pHoriBlurPS = pShCol->GetPixelShader(HORI_PS_NAME);

		// バッファ設定
		buffer.screenWidth = _width;
		buffer.screenHeight = _height;
	}

	const RenderTarget& GausianBlur::ApplyBlur(const Texture& _texture)
	{
		// 最初にクリアする
		pBlurRenderTarget->Clear();

		// レンダーターゲット
		pRenderer->SetRenderTerget(1, pBlurRenderTarget.get(), nullptr);

		// 縦ブラー
		// バッファ・テクスチャをセット
		pVertBlurPS->SetTexture(0, &_texture);
		pVertBlurPS->UpdateSubResource(0, &buffer);

		pGausianVS->SetGPU();
		pVertBlurPS->SetGPU();
		// フルスクリーン描画
		pRenderer->DrawFullScreenMesh();

		// 横ブラー
		pHoriBlurPS->SetTexture(0, &_texture);
		pHoriBlurPS->UpdateSubResource(0, &buffer);

		pHoriBlurPS->SetGPU();

		// フルスクリーン描画
		pRenderer->DrawFullScreenMesh();

		return *pBlurRenderTarget;
	}

	void GausianBlur::CreateRenderTarget(u_int _width, u_int _height)
	{
		pRenderer = Direct3D11::GetInstance()->GetRenderer();

		// レンダーターゲットを作成
		pBlurRenderTarget = std::make_unique<RenderTarget>();
		pBlurRenderTarget->Create(DXGI_FORMAT_R32G32B32A32_FLOAT,
			_width,
			_height);

		// シェーダーを取得
		ShaderCollection* pShCol = ShaderCollection::GetInstance();
		pGausianVS = pShCol->GetVertexShader(VS_NAME);
		pVertBlurPS = pShCol->GetPixelShader(VERT_PS_NAME);
		pHoriBlurPS = pShCol->GetPixelShader(HORI_PS_NAME);

		// バッファ設定
		buffer.screenWidth = pRenderer->GetWindowWidth();
		buffer.screenHeight = pRenderer->GetWindowHeight();
	}

	void GausianBlur::ImGuiDebug()
	{
		if (!ImGuiMethod::TreeNode("Blur RenderTarget")) return;
		// 描画
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)&pBlurRenderTarget->GetSRV(), viewportSize);
		ImGui::TreePop();
	}

}

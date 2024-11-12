#include "pch.h"
#include "ShadowDrawer.h"

void ShadowDrawer::Init()
{
	D3D11_Renderer* pRenderer = Direct3D11::GetInstance()->GetRenderer();
	u_int width = pRenderer->GetWindowWidth();
	u_int height = pRenderer->GetWindowHeight();

	// 深度書き込み用レンダーターゲット作成
	pRenderTarget = std::make_unique<RenderTarget>();
	pRenderTarget->Create(DXGI_FORMAT_R32_FLOAT, width, height);

	// 深度書き込み用レンダーターゲット作成
	pDepthStencil = std::make_unique<DepthStencil>();
	pDepthStencil->Create(width, height, false);
}

#include "pch.h"
#include "ShadowDrawer.h"

void ShadowDrawer::Init()
{
	D3D11_Renderer* pRenderer = Direct3D11::GetInstance()->GetRenderer();
	u_int width = pRenderer->GetWindowWidth();
	u_int height = pRenderer->GetWindowHeight();

	// �[�x�������ݗp�����_�[�^�[�Q�b�g�쐬
	pRenderTarget = std::make_unique<RenderTarget>();
	pRenderTarget->Create(DXGI_FORMAT_R32_FLOAT, width, height);

	// �[�x�������ݗp�����_�[�^�[�Q�b�g�쐬
	pDepthStencil = std::make_unique<DepthStencil>();
	pDepthStencil->Create(width, height, false);
}

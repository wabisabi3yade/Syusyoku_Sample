#include "pch.h"
#include "BlendState.h"
bool BlendState::Init(ID3D11Device& _pDevice)
{
	// RenderTarget0Ç÷ÇÃAlphaÉuÉåÉìÉhï`âÊê›íË
    D3D11_BLEND_DESC BlendState;
    ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
    BlendState.AlphaToCoverageEnable = FALSE;
    BlendState.IndependentBlendEnable = FALSE;
    BlendState.RenderTarget[0].BlendEnable = TRUE;
    BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;

    // îwåiêFÇ…ä|ÇØçáÇÌÇπÇÈåWêî
    BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	auto hr = _pDevice.CreateBlendState(&BlendState, &pBlendAlpha);
	if (FAILED(hr)) // é∏îs
	{
		return false;
	}

	return true;
}
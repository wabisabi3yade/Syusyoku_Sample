#include "pch.h"
#include "BlendState.h"

BlendState::BlendState(ID3D11Device* _device)
{
	// ���������s��
	bool isCheck = BlendStateInit(_device);
	// ���������s�Ȃ�
	if (!isCheck)
		ImGuiDebugLog::AddDebugLog("�u�����h�X�e�[�g�������ŃG���[");

}

BlendState::~BlendState()
{
	SAFE_RELEASE(pBlendStateObj);
}

bool BlendState::BlendStateInit(ID3D11Device* _pDevice)
{
	// RenderTarget0�ւ�Alpha�u�����h�`��ݒ�
	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	BlendState.AlphaToCoverageEnable = FALSE;
	BlendState.IndependentBlendEnable = FALSE;
	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	auto hr = _pDevice->CreateBlendState(&BlendState, &pBlendStateObj);
	if (FAILED(hr)) {
		//TRACE(L"InitDirect3D g_pD3DDevice->CreateBlendState", hr);
		return false;
	}

	return true;
}
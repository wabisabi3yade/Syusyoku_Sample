#include "pch.h"
#include "PixelShader.h"

void PixelShader::MakeShader(const char* _pData, u_int _dataSize)
{
	// ピクセルシェーダーを作成
	Direct3D11::GetInstance()->GetRenderer()->GetDevice()->
		CreatePixelShader(_pData, _dataSize, NULL, &pPxShader);
}

PixelShader::~PixelShader()
{
	SAFE_RELEASE(pPxShader);
}

void PixelShader::SetGPU()
{
	ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();
	// シェーダーを送る
	pContext->PSSetShader(pPxShader, nullptr, 0);
	// 定数バッファを送る
	for (int i = 0; i < pBuffers.size(); i++)
		pContext->PSSetConstantBuffers(i, 1, &pBuffers[i]);
	for (int i = 0; i < pTextures.size(); i++)
		pContext->PSSetShaderResources(i, 1, &pTextures[i]);
}
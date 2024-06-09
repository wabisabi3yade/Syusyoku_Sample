#include "pch.h"
#include "VertexShader.h"
#include "Direct3D11.h"

void VertexShader::MakeShader(const char* _pData, u_int _dataSize)
{
	// 頂点シェーダーを作成
	ID3D11VertexShader* ptr = pVxShader.get();
	Direct3D11::GetInstance()->GetRenderer()->GetDevice()
		->CreateVertexShader(_pData, _dataSize, nullptr, &ptr);
}

VertexShader::VertexShader() : Shader(Shader::Type::Vertex)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Bind()
{
	ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	// デバイスコンテキストに頂点シェーダーをセット
	pContext->VSSetShader(pVxShader.get(), nullptr, 0);
	// 定数バッファを送る
	for (int i = 0; i < pBuffers.size(); i++)
		pContext->VSSetConstantBuffers(i, 1, &pBuffers[i]);
	// リソースを送る
	for (int i = 0; i < pTextures.size(); i++)
		pContext->VSSetShaderResources(i, 1, &pTextures[i]);
}

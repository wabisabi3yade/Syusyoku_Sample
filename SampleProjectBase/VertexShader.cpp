#include "pch.h"
#include "VertexShader.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

void VertexShader::MakeShader(const char* _pData, u_int _dataSize)
{
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	ID3D11Device* pDevice = renderer->GetDevice();
	HRESULT hr;

	// 頂点シェーダーを作成
	hr = pDevice->CreateVertexShader(_pData, _dataSize, nullptr, &pVxShader);
	if (FAILED(hr))
	{
		HASHI_DEBUG_LOG("頂点シェーダー作成失敗");
		return;
	}

	MakeInputLayout(_pData, _dataSize);
}

void VertexShader::MakeInputLayout(const char* _pData, u_int _dataSize)
{
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	ID3D11Device* pDevice = renderer->GetDevice();

	// 入力レイアウトオブジェクト作成
	// →　頂点シェーダーにはどういったデータ構造の頂点データが渡ってくるかを定義した入力レイアウト
	// Vertex構造体のメンバ変数のサイズを指定して、シェーダーの何の情報なのかを判別する
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	HRESULT hr = pDevice->CreateInputLayout(
		layout,
		_countof(layout),
		_pData,
		_dataSize,
		&pInputLayout
	);

	if (FAILED(hr))
	{
		HASHI_DEBUG_LOG("失敗");
		return;
	}
}

VertexShader::VertexShader() : Shader(Shader::Type::Vertex), pVxShader(nullptr)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Bind()
{
	ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	// デバイスコンテキストに頂点シェーダーをセット
	pContext->VSSetShader(pVxShader.Get(), nullptr, 0);

	// 入力レイアウトをセット
	pContext->IASetInputLayout(pInputLayout.Get());

	// 定数バッファを送る
	for (int i = 0; i < pBuffers.size(); i++)
		pContext->VSSetConstantBuffers(i, 1, &pBuffers[i]);

	// リソースを送る
	for (int i = 0; i < pTextures.size(); i++)
		pContext->VSSetShaderResources(i, 1, &pTextures[i]);
}

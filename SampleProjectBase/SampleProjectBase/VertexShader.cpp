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
		ImGuiDebugLog::Add("頂点シェーダー作成失敗");
		return;
	}

	///*
	//シェーダ作成時にシェーダリフレクションを通してインプットレイアウトを取得
	//セマンティクスの配置などから識別子を作成
	//識別子が登録済→再利用、なければ新規作成
	//https://blog.techlab-xe.net/dxc-shader-reflection/
	//*/

	//ID3D11ShaderReflection* pReflection;
	//D3D11_SHADER_DESC shaderDesc;
	//D3D11_INPUT_ELEMENT_DESC* pInputDesc;
	//D3D11_SIGNATURE_PARAMETER_DESC sigDesc;

	//DXGI_FORMAT formats[][4] =
	//{
	//	{
	//		DXGI_FORMAT_R32_UINT,
	//		DXGI_FORMAT_R32G32_UINT,
	//		DXGI_FORMAT_R32G32B32_UINT,
	//		DXGI_FORMAT_R32G32B32A32_UINT,
	//	}, {
	//		DXGI_FORMAT_R32_SINT,
	//		DXGI_FORMAT_R32G32_SINT,
	//		DXGI_FORMAT_R32G32B32_SINT,
	//		DXGI_FORMAT_R32G32B32A32_SINT,
	//	}, {
	//		DXGI_FORMAT_R32_FLOAT,
	//		DXGI_FORMAT_R32G32_FLOAT,
	//		DXGI_FORMAT_R32G32B32_FLOAT,
	//		DXGI_FORMAT_R32G32B32A32_FLOAT,
	//	},
	//};

	//hr = D3DReflect(_pData, _dataSize, IID_PPV_ARGS(&pReflection));
	//if (FAILED(hr)) 
	//{ 
	//	ImGuiDebugLog::Add("リフレクション情報作成で失敗");
	//	return; 
	//}

	//pReflection->GetDesc(&shaderDesc);
	//pInputDesc = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.InputParameters];
	//for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
	//{
	//	pReflection->GetInputParameterDesc(i, &sigDesc);
	//	pInputDesc[i].SemanticName = sigDesc.SemanticName;
	//	pInputDesc[i].SemanticIndex = sigDesc.SemanticIndex;

	//	// http://marupeke296.com/TIPS_No17_Bit.html
	//	BYTE elementCount = sigDesc.Mask;
	//	elementCount = (elementCount & 0x05) + ((elementCount >> 1) & 0x05);
	//	elementCount = (elementCount & 0x03) + ((elementCount >> 2) & 0x03);

	//	switch (sigDesc.ComponentType)
	//	{
	//	case D3D_REGISTER_COMPONENT_UINT32:
	//		pInputDesc[i].Format = formats[0][elementCount - 1];
	//		break;
	//	case D3D_REGISTER_COMPONENT_SINT32:
	//		pInputDesc[i].Format = formats[1][elementCount - 1];
	//		break;
	//	case D3D_REGISTER_COMPONENT_FLOAT32:
	//		pInputDesc[i].Format = formats[2][elementCount - 1];
	//		break;
	//	}
	//	pInputDesc[i].InputSlot = 0;
	//	pInputDesc[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
	//	pInputDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//	pInputDesc[i].InstanceDataStepRate = 0;
	//}

	// //インプットレイアウト作成
	//ID3D11InputLayout* pInputLayout = nullptr;
	//hr = pDevice->CreateInputLayout(
	//	pInputDesc, shaderDesc.InputParameters,
	//	_pData, _dataSize, &pInputLayout
	//);
	//if (FAILED(hr))
	//{
	//	ImGuiDebugLog::Add("インプットレイアウト作成失敗");
	//	delete[] pInputDesc;
	//	return;
	//}
	//renderer->GetParameter().SetInputLayout(pInputLayout);	// レンダラーにインプットレイアウトを渡す

	//delete[] pInputDesc;



	// 入力レイアウトオブジェクト作成
	// →　頂点シェーダーにはどういったデータ構造の頂点データが渡ってくるかを定義した入力レイアウト
	ID3D11InputLayout* pInputLayout = nullptr;
	// Vertex構造体のメンバ変数のサイズを指定して、シェーダーの何の情報なのかを判別する
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	hr = pDevice->CreateInputLayout(
		layout,
		_countof(layout),
		_pData,
		_dataSize,
		&pInputLayout
	);
	renderer->GetParameter().SetInputLayout(pInputLayout);	// レンダラーにインプットレイアウトを渡す
	if (FAILED(hr))
	{
		ImGuiDebugLog::Add("失敗");
	};
}

VertexShader::VertexShader() : Shader(Shader::Type::Vertex), pVxShader(nullptr)
{
}

VertexShader::~VertexShader()
{
	SAFE_RELEASE(pVxShader);
}

void VertexShader::Bind()
{
	ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	// デバイスコンテキストに頂点シェーダーをセット
	pContext->VSSetShader(pVxShader, nullptr, 0);
	// 定数バッファを送る
	for (int i = 0; i < pBuffers.size(); i++)
		pContext->VSSetConstantBuffers(i, 1, &pBuffers[i]);
	// リソースを送る
	for (int i = 0; i < pTextures.size(); i++)
		pContext->VSSetShaderResources(i, 1, &pTextures[i]);
}

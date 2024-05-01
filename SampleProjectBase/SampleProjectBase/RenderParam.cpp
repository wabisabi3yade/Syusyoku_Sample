#include "pch.h"

#include "D3D11_Renderer.h"
#include "RenderParam.h"


RenderParam::~RenderParam()
{
}

bool RenderParam::InitConstBuffer(ID3D11Device* _device)
{
	// 定数バッファの定義
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// 定数バッファ�@の作成
	// GPUに渡すデータサイズを取得
	cbDesc.ByteWidth = sizeof(CbTransform);
	auto hr = _device->CreateBuffer(
		&cbDesc, 
		nullptr, 
		&cbTransformSet.pBuffer);
	if (FAILED(hr))
	{
		MessageError("座標定数バッファの初期化でエラー");
		return false;
	}

	cbDesc.ByteWidth = sizeof(CbView);
	hr = _device->CreateBuffer(
		&cbDesc,
		nullptr,
		&cbViewSet.pBuffer);
	if (FAILED(hr))
	{
		MessageError("ビュー変換定数バッファの初期化でエラー");
		return false;
	}

	cbDesc.ByteWidth = sizeof(CbProjection);
	hr = _device->CreateBuffer(
		&cbDesc,
		nullptr,
		&cbProjectionSet.pBuffer);

	if (FAILED(hr)) 
	{
		MessageError("プロジェクション定数バッファの初期化でエラー");
		return false;
	}

	return true;
}

bool RenderParam::Init(ID3D11Device* _device)
{
	bool result = InitConstBuffer(_device);

	if (!result)
	{
		MessageError("RenderParamクラスの初期化でエラー");
		return false;
	}

	return true;
}


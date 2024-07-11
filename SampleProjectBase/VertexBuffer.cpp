#include "pch.h"
#include "VertexBuffer.h"

bool VertexBuffer::CreateBuffer(UINT size, void* pInitData)
{
	auto pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

	D3D11_BUFFER_DESC desc;

	// 頂点バッファの初期化設定
	desc.ByteWidth = size;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;

	// 初期化時に設定するバッファデータ
	data.pSysMem = pInitData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	// 頂点バッファの生成
	ID3D11Buffer* buffer = pBuffer.Get();
	HRESULT hr = pDevice->CreateBuffer(&desc, &data, &buffer);
	
	if (FAILED(hr))
		return false;
	
	return true;
}

const ID3D11Buffer& VertexBuffer::GetBuffer()const
{
	assert(pBuffer != NULL || "頂点バッファ取得失敗");

	return *pBuffer.Get();
}

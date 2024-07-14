#include "pch.h"
#include "VertexBuffer.h"

bool VertexBuffer::CreateBuffer(u_int _allSize, u_int _elementSize, void* _pInitData)
{
	auto pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

	D3D11_BUFFER_DESC desc;

	// 頂点バッファの初期化設定
	desc.ByteWidth = _allSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;

	// 初期化時に設定するバッファデータ
	data.pSysMem = _pInitData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	// 頂点バッファの生成
	HRESULT hr = pDevice->CreateBuffer(&desc, &data, &pBuffer);
	
	if (FAILED(hr))
		return false;
	
	return true;
}

void VertexBuffer::SetGPU() const
{
	assert(pBuffer != NULL && "GPU送信で頂点バッファがNULL");

	// デバイスコンテキスト取得
	ID3D11DeviceContext* pDevicecontext = 
		Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	// 頂点バッファをセットする
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	pDevicecontext->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
}


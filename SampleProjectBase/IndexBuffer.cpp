#include "pch.h"
#include "IndexBuffer.h"

bool IndexBuffer::CreateBuffer(u_int _allSize, u_int _elementSize, void* pInitData)
{
	auto pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

	D3D11_BUFFER_DESC desc;

	// 頂点バッファの初期化設定
	desc.ByteWidth = _allSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;

	// 初期化時に設定するバッファデータ
	data.pSysMem = pInitData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	// 頂点バッファの生成
	HRESULT hr = pDevice->CreateBuffer(&desc, &data, &pBuffer);

	if (FAILED(hr))
		return false;

	return true;
}

void IndexBuffer::SetGPU() const
{
	assert(pBuffer != NULL && "GPU送信でインデックスバッファがNULL");

	ID3D11DeviceContext* pDevicecontext =
		Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	// インデックスバッファをセット
	pDevicecontext->IASetIndexBuffer(pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}
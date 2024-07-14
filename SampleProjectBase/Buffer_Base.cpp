#include "pch.h"
#include "Buffer_Base.h"

const ID3D11Buffer& Buffer_Base::GetBuffer() const
{
	assert(pBuffer != NULL || "頂点バッファ取得失敗");

	return *pBuffer.Get();
}

void Buffer_Base::UpdateBuffer(void* _updateData) const
{
	ID3D11DeviceContext* pDeviceContext
		= Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	// バッファを更新する
	pDeviceContext->UpdateSubresource(
		pBuffer.Get(),
		0,
		nullptr,
		&_updateData,
		0,
		0
	);
}


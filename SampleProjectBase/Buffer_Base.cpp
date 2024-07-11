#include "pch.h"
#include "Buffer_Base.h"

const ID3D11Buffer& Buffer_Base::GetBuffer() const
{
	assert(pBuffer != NULL || "頂点バッファ取得失敗");

	return *pBuffer.Get();
}

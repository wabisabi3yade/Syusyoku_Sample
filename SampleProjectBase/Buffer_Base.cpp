#include "pch.h"
#include "Buffer_Base.h"

const ID3D11Buffer& Buffer_Base::GetBuffer() const
{
	assert(pBuffer != NULL || "���_�o�b�t�@�擾���s");

	return *pBuffer.Get();
}

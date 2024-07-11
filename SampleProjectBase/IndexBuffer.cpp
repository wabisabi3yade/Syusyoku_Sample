#include "pch.h"
#include "IndexBuffer.h"

bool IndexBuffer::CreateBuffer(u_int _allSize, u_int _elementSize, void* pInitData)
{
	auto pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

	D3D11_BUFFER_DESC desc;

	// ���_�o�b�t�@�̏������ݒ�
	desc.ByteWidth = _allSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;

	// ���������ɐݒ肷��o�b�t�@�f�[�^
	data.pSysMem = pInitData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	// ���_�o�b�t�@�̐���
	HRESULT hr = pDevice->CreateBuffer(&desc, &data, &pBuffer);

	if (FAILED(hr))
		return false;

	return true;
}
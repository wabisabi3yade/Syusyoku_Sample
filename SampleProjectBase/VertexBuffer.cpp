#include "pch.h"
#include "VertexBuffer.h"

bool VertexBuffer::CreateBuffer(UINT size, void* pInitData)
{
	auto pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

	D3D11_BUFFER_DESC desc;

	// ���_�o�b�t�@�̏������ݒ�
	desc.ByteWidth = size;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;

	// ���������ɐݒ肷��o�b�t�@�f�[�^
	data.pSysMem = pInitData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	// ���_�o�b�t�@�̐���
	ID3D11Buffer* buffer = pBuffer.Get();
	HRESULT hr = pDevice->CreateBuffer(&desc, &data, &buffer);
	
	if (FAILED(hr))
		return false;
	
	return true;
}

const ID3D11Buffer& VertexBuffer::GetBuffer()const
{
	assert(pBuffer != NULL || "���_�o�b�t�@�擾���s");

	return *pBuffer.Get();
}

#include "pch.h"
#include "VertexBuffer.h"

bool VertexBuffer::CreateBuffer(u_int _allSize, u_int _elementSize, void* _pInitData)
{
	auto pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

	D3D11_BUFFER_DESC desc;

	// ���_�o�b�t�@�̏������ݒ�
	desc.ByteWidth = _allSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;

	// ���������ɐݒ肷��o�b�t�@�f�[�^
	data.pSysMem = _pInitData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	// ���_�o�b�t�@�̐���
	HRESULT hr = pDevice->CreateBuffer(&desc, &data, &pBuffer);
	
	if (FAILED(hr))
		return false;
	
	return true;
}

void VertexBuffer::SetGPU() const
{
	assert(pBuffer != NULL && "GPU���M�Œ��_�o�b�t�@��NULL");

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* pDevicecontext = 
		Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	// ���_�o�b�t�@���Z�b�g����
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	pDevicecontext->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
}


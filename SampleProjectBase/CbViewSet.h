#pragma once

// �r���[�ϊ��s��
struct CbView
{
	DirectX::XMFLOAT4X4 view = {};
};

// GPU�ɓn���r���[�ϊ��s��ƃo�b�t�@
struct CbViewSet
{
	CbView data;

	ID3D11Buffer* pBuffer = nullptr;
};
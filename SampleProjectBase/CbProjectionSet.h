#pragma once

// ���e�s��
struct CbProjection
{
	DirectX::XMFLOAT4X4  projection = {};
};

// GPU�ɓn���v���W�F�N�V�����s��ƒ萔�o�b�t�@���Z�b�g�ɂ����\����
struct CbProjectionSet
{
	// ���_���W�Ȃǂ̃f�[�^
	CbProjection data;
	// �萔�o�b�t�@
	ID3D11Buffer* pBuffer = nullptr;
};
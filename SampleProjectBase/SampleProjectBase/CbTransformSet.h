#pragma once

// GPU�ɓn�����_�f�[�^���̏��
struct CbTransform
{
	DirectX::XMFLOAT4X4 transform = {};
};


// GPU�ɓn�����W�f�[�^�ƒ萔�o�b�t�@���Z�b�g�ɂ����\����
struct CbTransformSet
{
	// ���_���W�Ȃǂ̃f�[�^
	CbTransform data;
	// �萔�o�b�t�@
	ID3D11Buffer* pBuffer = nullptr;
};
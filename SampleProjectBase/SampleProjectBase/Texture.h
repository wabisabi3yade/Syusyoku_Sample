#pragma once

// �e�N�X�`���N���X
class Texture
{
	// �V�F�[�_�[���\�[�X�r���[(SRV)
	// �e�N�X�`���A�o�b�t�@�Ȃǂ̃��\�[�X���V�F�[�_�[�ŎQ�Ɖ\�Ȍ`��
	ID3D11ShaderResourceView* pSRV;

	// �摜(���E�c�T�C�Y)
	u_int width;
	u_int height;

	// ��������
	bool Setup(const char* _pathName);

public:
	Texture(const char* _pathName);
	~Texture();
};


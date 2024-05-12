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

public:

	// �t�@�C���`�����w�肵���`���ɕύX����
	static const wchar_t* ReplaceExtension(const std::wstring& _pathName, const char* ext);

	Texture();
	Texture(const char* _pathName, bool& _isSucess);
	// ��������
	bool Setup(const char* _pathName);

	void Release();

	ID3D11ShaderResourceView* GetSRV() { return pSRV; }

	u_int GetWidth()const { return width; }	// �����擾
	u_int GetHeight()const { return height; }	// �������擾

	~Texture();
};


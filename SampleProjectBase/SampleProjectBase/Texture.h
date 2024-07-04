#pragma once

// �e�N�X�`���N���X
class Texture
{	bool isImported;	// ���f����񂪓��������ǂ���

	// �摜(���E�c�T�C�Y)
	u_int width;
	u_int height;

	// �V�F�[�_�[���\�[�X�r���[(SRV)
	// �e�N�X�`���A�o�b�t�@�Ȃǂ̃��\�[�X���V�F�[�_�[�ŎQ�Ɖ\�Ȍ`��
	ID3D11ShaderResourceView* pSRV;
public:
	// �t�@�C���`�����w�肵���`���ɕύX����
	static const wchar_t* ReplaceExtension(const std::wstring& _pathName, const char* ext);

	Texture();
	// ��������
	bool Load(const char* _pathName);

	void Release();

	ID3D11ShaderResourceView* GetSRV()const { return pSRV; }

	u_int GetWidth()const { return width; }	// �����擾
	u_int GetHeight()const { return height; }	// �������擾

	~Texture();
};


#pragma once
#include "Asset_Base.h"

/// @brief �e�N�X�`���N���X
class Texture : public Asset_Base
{
	friend class AssetLoader;

	/// @brief �e�N�X�`���A�o�b�t�@�Ȃǂ̃��\�[�X���V�F�[�_�[�ŎQ�Ɖ\�Ȍ`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV;

	/// @brief �摜(���E�c�T�C�Y)
	u_int width;
	u_int height;
public:
	Texture() : width(0), height(0) {}
	~Texture() {}

	/// @brief SRV�̎Q�Ƃ��擾
	/// @return SRV�̎Q��
	ID3D11ShaderResourceView& GetSRV()const { return *pSRV.Get(); }

	/// @brief �����擾
	/// @return �摜�̉���(px)
	u_int GetWidth()const { return width; }

	/// @brief �������擾
	/// @return �摜�̍���(px)
	u_int GetHeight()const { return height; }
};

/// @brief �e�N�X�`����Null�I�u�W�F�N�g�N���X
class NullTexture : public Texture
{
public:
	NullTexture() {}
	~NullTexture() {}

	/// @brief �����擾
	/// @return �摜�̉���(px)
	u_int GetWidth()const { return 0; }

	/// @brief �������擾
	/// @return �摜�̍���(px)
	u_int GetHeight()const { return 0; }
};


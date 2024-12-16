#pragma once
#include "AssetPath_Base.h"

namespace HashiTaku
{
	/// @brief �e�N�X�`���N���X
	class Texture : public AssetPath_Base
	{
		friend class AssetLoader;

	protected:
		/// @brief �e�N�X�`���A�o�b�t�@�Ȃǂ̃��\�[�X���V�F�[�_�[�ŎQ�Ɖ\�Ȍ`��
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV;

		/// @brief �摜(���E�c�T�C�Y)
		u_int width;
		u_int height;
	public:
		Texture() : width(0), height(0) {}
		~Texture() {}

		// SRV���Z�b�g
		virtual void SetSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pSRV);

		// �������Z�b�g
		virtual void SetWidth(u_int _width);

		// �c�����Z�b�g
		virtual void SetHeight(u_int _height);

		/// @brief SRV�̎Q�Ƃ��擾
		/// @return SRV�̎Q��
		virtual ID3D11ShaderResourceView& GetSRV()const { return *pSRV.Get(); }

		/// @brief �����擾
		/// @return �摜�̉���(px)
		virtual u_int GetWidth()const { return width; }

		/// @brief �������擾
		/// @return �摜�̍���(px)
		virtual u_int GetHeight()const { return height; }

		virtual bool GetIsSetSRV() const;
	};

	/// @brief �e�N�X�`����Null�I�u�W�F�N�g�N���X
	class NullTexture : public Texture
	{
	public:
		NullTexture() {}
		~NullTexture() {}

		// SRV���Z�b�g
		void SetSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pSRV) override {}

		// �������Z�b�g
		void SetWidth(u_int _width) override {}

		// �c�����Z�b�g
		void SetHeight(u_int _height) override {}

		/// @brief �����擾
		/// @return �摜�̉���(px)
		u_int GetWidth()const override { return 0; }

		/// @brief �������擾
		/// @return �摜�̍���(px)
		u_int GetHeight()const override { return 0; }

		bool GetIsSetSRV() const override { return false; }
	};
}
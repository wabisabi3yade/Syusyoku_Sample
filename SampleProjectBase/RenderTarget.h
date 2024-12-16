#pragma once
#include "Texture.h"

namespace HashiTaku
{
	/// @brief �����_�[�^�[�Q�b�g��[�x�e�N�X�`���̊��
	class D3DTexture_Base : public Texture
	{
	protected:
		/// @brief �e�N�X�`��
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

	public:
		D3DTexture_Base();
		virtual ~D3DTexture_Base() {}

	protected:
		/// @brief Description�쐬
		/// @param _format �t�H�[�}�b�g
		/// @param _width ����
		/// @param _height ����
		/// @return �쐬����Description
		D3D11_TEXTURE2D_DESC MakeTexDesc(DXGI_FORMAT _format, UINT _width, UINT _height);

		/// @brief �e���\�[�X�쐬
		/// @param _desc desc
		/// @param _pData �f�[�^
		/// @return �����������H
		virtual bool CreateResource(D3D11_TEXTURE2D_DESC& _desc, const void* _pData = nullptr);
	};

	/// @brief �����_�[�^�[�Q�b�g�N���X
	class RenderTarget : public D3DTexture_Base
	{
		/// @brief  �����_�[�^�[�Q�b�g�r���[
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTV;
	public:
		RenderTarget();
		~RenderTarget() {}

		/// @brief �����_�[�^�[�Q�b�g�쐬
		/// @param _format desc�̃t�H�[�}�b�g
		/// @param _width ����
		/// @param _height ����
		/// @return �쐬�����������H
		bool Create(DXGI_FORMAT _format, UINT _width, UINT _height);

		/// @brief �����_�[�^�[�Q�b�g�r���[���擾
		/// @return �����_�[�^�[�Q�b�g�r���[
		ID3D11RenderTargetView* GetView() const;

	private:
		/// @brief ���\�[�X�쐬
		/// @param _desc desc
		/// @param pData �f�[�^
		/// @return �����������H
		bool CreateResource(D3D11_TEXTURE2D_DESC& _desc, const void* pData = nullptr) override;
	};
}
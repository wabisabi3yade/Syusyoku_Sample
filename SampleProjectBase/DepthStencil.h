#pragma once
#include "RenderTarget.h"

namespace HashiTaku
{
	// �[�x�X�e���V���N���X
	class DepthStencil : public D3DTexture_Base
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

	public:
		DepthStencil() {}
		~DepthStencil() {}

		/// @brief �N���A����
		void ClearColor();

		/// @brief �쐬����
		/// @param _width ����
		/// @param _height �c��
		/// @param _useStencil 
		/// @return �쐬�����������H
		bool Create(UINT _width, UINT _height, bool _useStencil);

		ID3D11DepthStencilView* GetView() const;

	private:
		bool CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr) override;
	};
}

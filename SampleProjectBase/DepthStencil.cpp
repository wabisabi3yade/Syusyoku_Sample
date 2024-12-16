#include "pch.h"
#include "DepthStencil.h"

namespace HashiTaku
{
	void DepthStencil::ClearColor()
	{
		ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	bool DepthStencil::Create(UINT _width, UINT _height, bool _useStencil)
	{
		// description�쐬
		D3D11_TEXTURE2D_DESC desc = MakeTexDesc(
			_useStencil ? DXGI_FORMAT_R24G8_TYPELESS :
			DXGI_FORMAT_R32_TYPELESS,
			width, height
		);

		desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;

		// �쐬
		return CreateResource(desc);
	}

	ID3D11DepthStencilView* DepthStencil::GetView() const
	{
		return pDSV.Get();
	}

	bool DepthStencil::CreateResource(D3D11_TEXTURE2D_DESC& _desc, const void* _pData)
	{
		ID3D11Device* pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

		// �X�e���V���g�p����
		bool useStencil = (_desc.Format == DXGI_FORMAT_R24G8_TYPELESS);

		// ���\�[�X����
		_desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		bool isSuccess = D3DTexture_Base::CreateResource(_desc, nullptr);

		if (!isSuccess)
		{
			assert(!"�e�N�X�`���쐬���s");
			return false;
		}

		// �ݒ�
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		// ����
		HRESULT hr = pDevice->CreateDepthStencilView(pTexture.Get(), &dsvDesc, pDSV.GetAddressOf());

		if (FAILED(hr))
		{
			assert(!"�[�x�e�N�X�`���쐬���s");
			return false;
		}

		return true;
	}
}
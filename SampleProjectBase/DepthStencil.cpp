#include "pch.h"
#include "DepthStencil.h"

void DepthStencil::Clear()
{
	ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

bool DepthStencil::Create(UINT _width, UINT _height, bool _useStencil)
{
	ID3D11Device* pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

	DXGI_FORMAT format = _useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Width = _width;
	desc.Height = _height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;

	
	HRESULT hr = E_FAIL;
	// テクスチャ作成
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D;
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = nullptr;
	data.SysMemPitch = desc.Width * 4;
	hr = pDevice->CreateTexture2D(&desc, nullptr, pTexture2D.GetAddressOf());
	if (FAILED(hr))
	{
		assert(!"ステンシルテクスチャ作成失敗");
		return false; 
	}

	// 設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	switch (desc.Format)
	{
	default:						srvDesc.Format = desc.Format;			break;
	case DXGI_FORMAT_R32_TYPELESS: 	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;	break;
	}
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// 生成
	hr = pDevice->CreateShaderResourceView(pTexture2D.Get(), &srvDesc, pSRV.GetAddressOf());
	if (SUCCEEDED(hr))
	{
		width = desc.Width;
		height = desc.Height;
	}
	else if (FAILED(hr))
	{
		assert(!"ステンシルSRV作成失敗");
		return false;
	}

	// 設定
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = _useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	// 生成
	pDevice->CreateDepthStencilView(pTexture2D.Get(), &dsvDesc, pDSV.GetAddressOf());

	return true;
}

ID3D11DepthStencilView* DepthStencil::GetView() const
{
	return pDSV.Get();
}

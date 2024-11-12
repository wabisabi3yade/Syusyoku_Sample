#include "pch.h"
#include "RenderTarget.h"

using namespace DirectX::SimpleMath;


D3DTexture_Base::D3DTexture_Base()
{
}

D3D11_TEXTURE2D_DESC D3DTexture_Base::MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	return desc;
}

bool D3DTexture_Base::CreateResource(D3D11_TEXTURE2D_DESC& _desc, const void* pData)
{
	ID3D11Device* pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

	// テクスチャリソース作成
	HRESULT hr = E_FAIL;

	// テクスチャ作成
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = pData;
	data.SysMemPitch = _desc.Width * 4;
	hr = pDevice->CreateTexture2D(&_desc, pData ? &data : nullptr, pTexture.GetAddressOf());
	if (FAILED(hr))
		return false;

	// 設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	switch (_desc.Format)
	{
	case DXGI_FORMAT_R32_TYPELESS:
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		break;

	default:
		srvDesc.Format = _desc.Format;
		break;
	}

	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// 生成
	hr = pDevice->CreateShaderResourceView(pTexture.Get(), &srvDesc, pSRV.GetAddressOf());

	// 成功したなら
	if (SUCCEEDED(hr))
	{
		width = _desc.Width;
		height = _desc.Height;
	}
	else
		return false;

	return true;
}

RenderTarget::RenderTarget()
{
}

bool RenderTarget::Create(DXGI_FORMAT format, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);
	desc.BindFlags |= D3D11_BIND_RENDER_TARGET;

	return CreateResource(desc);
}

ID3D11RenderTargetView* RenderTarget::GetView() const
{
	return pRTV.Get();
}

bool RenderTarget::CreateResource(D3D11_TEXTURE2D_DESC& _desc, const void* _pData)
{
	ID3D11Device* pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

	// テクスチャリソース作成
	HRESULT hr = E_FAIL;

	bool isSuccess = D3DTexture_Base::CreateResource(_desc, nullptr);
	if (!isSuccess)
	{
		assert(!"リソース作成作成失敗");
		return false;
	}

	// 設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = _desc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	// 生成
	hr = pDevice->CreateRenderTargetView(pTexture.Get(), &rtvDesc, &pRTV);
	if (FAILED(hr))
	{
		assert(!"リソース作成作成失敗");
		return false;
	}
	
	return true;
}


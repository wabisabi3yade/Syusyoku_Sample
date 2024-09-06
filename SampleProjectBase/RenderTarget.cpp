#include "pch.h"
#include "RenderTarget.h"

using namespace DirectX::SimpleMath;

void RenderTarget::Clear()
{
	Clear(Color(0.0f, 0.0f, 0.0f, 0.0f));
}

void RenderTarget::Clear(DirectX::SimpleMath::Color color)
{
	ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();
	pContext->ClearRenderTargetView(pRTV.Get(), color);
}

bool RenderTarget::Create(DXGI_FORMAT format, UINT width, UINT height)
{
	return true;
}

bool RenderTarget::CreateFromScreen()
{
	ID3D11Device* pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();
	IDXGISwapChain* pSwapChain = Direct3D11::GetInstance()->GetRenderer()->GetSwapChain();

	HRESULT hr;

	// バックバッファのポインタを取得
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pTexture);
	if (FAILED(hr))
	{
		return false;
	}
	// バックバッファへのポインタを指定してレンダーターゲットビューを作成
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateRenderTargetView(pTexture.Get(), &rtvDesc, pRTV.GetAddressOf());
	if (SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC desc;
		pTexture->GetDesc(&desc);
		width = desc.Width;
		height = desc.Height;
	}
	else if (FAILED(hr))
	{
		assert(!"レンダーターゲットビュー作成失敗");
		return false;
	}

	return true;
}

ID3D11RenderTargetView* RenderTarget::GetView() const
{
	return pRTV.Get();
}

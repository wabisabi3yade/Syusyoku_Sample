#include "pch.h"
#include "Texture.h"
#include "Direct3D11.h"
#include "DirectXTex/TextureLoad.h"

bool Texture::Setup(const char* _pathName)
{
	// マルチバイト文字列をワイド文字列に変換
	wchar_t widePath[MAX_PATH];
	size_t wideLen = 0;
	MultiByteToWideChar(0, 0, _pathName, -1, widePath, MAX_PATH);

	// ファイル読込
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	HRESULT hr;

	// 読み込まれたファイルがtga形式なら
	if (strstr(_pathName, ".tga"))
	{
		hr = DirectX::LoadFromTGAFile(widePath, &metaData, image);
	}
	else
	{
		// Windowsに対応した画像形式(bmp, png, gif, tiff, jpegなど)を読み込む
		hr = DirectX::LoadFromWICFile(widePath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metaData, image);
	}

	if (FAILED(hr))
	{
		MessageError("画像読込でエラー");
		return false;
	}

	// SRVを作成
	ID3D11Device* device = Direct3D11::GetInstance()->GetRenderer()->GetDevice();
	hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metaData, &pSRV);

	if (FAILED(hr))
	{
		MessageError("SRV作成でエラー");
		return false;
	}
	else if (SUCCEEDED(hr))
	{
		width = metaData.width;
		height = metaData.height;
	}

	return true;
}

Texture::Texture(const char* _pathName) : width(0), height(0), pSRV(nullptr)
{
	Setup(_pathName);
}

Texture::~Texture()
{
	SAFE_RELEASE(pSRV);
}

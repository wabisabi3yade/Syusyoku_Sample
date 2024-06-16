#include "pch.h"
#include<filesystem>

#include "Texture.h"
#include "DirectXTex/TextureLoad.h"

bool Texture::Load(const char* _pathName)
{
	if (isImported)
	{
		ImGuiDebugLog::Add("既にこのTextureはロードされています");
		return true;
	}
	else if (_pathName == "")
	{
		ImGuiDebugLog::Add("テクスチャのパス名が入力されていません");
		return false;
	}

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
	// 読み込まれたファイルがpsd形式なら
	else if (strstr(_pathName, ".psd"))
	{
		std::string message = "psdファイルは対応していません\n" + std::string(_pathName);
		ImGuiDebugLog::Add(message);
	}
	else
	{
		// Windowsに対応した画像形式(bmp, png, gif, tiff, jpegなど)を読み込む
		hr = DirectX::LoadFromWICFile(widePath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metaData, image);
	}

	if (FAILED(hr))
	{
		return false;
	}

	// SRVを作成
	ID3D11Device* device = Direct3D11::GetInstance()->GetRenderer()->GetDevice();
	hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metaData, &pSRV);

	if (FAILED(hr))
	{
		ImGuiDebugLog::Add("SRV作成でエラー");
		return false;
	}
	else if (SUCCEEDED(hr))
	{
		width = metaData.width;
		height = metaData.height;
	}

	return true;
}

void Texture::Release()
{
	SAFE_RELEASE(pSRV);
}

const wchar_t* Texture::ReplaceExtension(const std::wstring& _pathName, const char* ext)
{
	std::filesystem::path p = _pathName.c_str();

	return p.replace_extension(ext).c_str();
}

Texture::Texture() : width(0), height(0), pSRV(nullptr), isImported(false), isPermanent(false)
{
}

Texture::~Texture()
{
	Release();
}

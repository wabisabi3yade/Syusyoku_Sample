#include "pch.h"
#include "TextureLoader.h"

#include<filesystem>
#include "DirectXTex/TextureLoad.h"

#include "Texture.h"

bool TextureLoader::Load(const char* _filePath)
{
	// テクスチャをロードする
	std::unique_ptr<Texture> pTex = std::make_unique<Texture>();

	if (_filePath == "")
	{
		ImGuiDebugLog::Add("テクスチャのパス名が入力されていません");
		return false;
	}

	// マルチバイト文字列をワイド文字列に変換
	wchar_t widePath[MAX_PATH];
	size_t wideLen = 0;
	MultiByteToWideChar(0, 0, _filePath, -1, widePath, MAX_PATH);

	// ファイル読込
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	HRESULT hr;

	// 読み込まれたファイルがtga形式なら
	if (strstr(_filePath, ".tga"))
	{
		hr = DirectX::LoadFromTGAFile(widePath, &metaData, image);
	}
	// 読み込まれたファイルがpsd形式なら
	else if (strstr(_filePath, ".psd"))
	{
		std::string message = "psdファイルは対応していません\n" + std::string(_filePath);
		ImGuiDebugLog::Add(message);

		return false;
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
	hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metaData, &pTex->pSRV);

	if (FAILED(hr))
	{
		ImGuiDebugLog::Add("SRV作成でエラー");
		return false;
	}
	else if (SUCCEEDED(hr))
	{
		pTex->width = static_cast<u_int>(metaData.width);
		pTex->height = static_cast<u_int>(metaData.height);
	}

	return true;
}

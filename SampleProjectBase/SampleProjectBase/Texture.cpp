#include "pch.h"
#include<filesystem>

#include "Texture.h"
#include "DirectXTex/TextureLoad.h"

bool Texture::Load(const char* _pathName)
{
	if (isImported)
	{
		ImGuiDebugLog::Add("���ɂ���Texture�̓��[�h����Ă��܂�");
		return true;
	}
	else if (_pathName == "")
	{
		ImGuiDebugLog::Add("�e�N�X�`���̃p�X�������͂���Ă��܂���");
		return false;
	}

	// �}���`�o�C�g����������C�h������ɕϊ�
	wchar_t widePath[MAX_PATH];
	size_t wideLen = 0;
	MultiByteToWideChar(0, 0, _pathName, -1, widePath, MAX_PATH);

	// �t�@�C���Ǎ�
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	HRESULT hr;

	// �ǂݍ��܂ꂽ�t�@�C����tga�`���Ȃ�
	if (strstr(_pathName, ".tga"))
	{
		hr = DirectX::LoadFromTGAFile(widePath, &metaData, image);
	}
	// �ǂݍ��܂ꂽ�t�@�C����psd�`���Ȃ�
	else if (strstr(_pathName, ".psd"))
	{
		std::string message = "psd�t�@�C���͑Ή����Ă��܂���\n" + std::string(_pathName);
		ImGuiDebugLog::Add(message);
	}
	else
	{
		// Windows�ɑΉ������摜�`��(bmp, png, gif, tiff, jpeg�Ȃ�)��ǂݍ���
		hr = DirectX::LoadFromWICFile(widePath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metaData, image);
	}

	if (FAILED(hr))
	{
		return false;
	}

	// SRV���쐬
	ID3D11Device* device = Direct3D11::GetInstance()->GetRenderer()->GetDevice();
	hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metaData, &pSRV);

	if (FAILED(hr))
	{
		ImGuiDebugLog::Add("SRV�쐬�ŃG���[");
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

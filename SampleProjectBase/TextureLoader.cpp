#include "pch.h"
#include "TextureLoader.h"

#include<filesystem>
#include "DirectXTex/TextureLoad.h"

#include "Texture.h"

bool TextureLoader::Load(const char* _filePath)
{
	// �e�N�X�`�������[�h����
	std::unique_ptr<Texture> pTex = std::make_unique<Texture>();

	if (_filePath == "")
	{
		ImGuiDebugLog::Add("�e�N�X�`���̃p�X�������͂���Ă��܂���");
		return false;
	}

	// �}���`�o�C�g����������C�h������ɕϊ�
	wchar_t widePath[MAX_PATH];
	size_t wideLen = 0;
	MultiByteToWideChar(0, 0, _filePath, -1, widePath, MAX_PATH);

	// �t�@�C���Ǎ�
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	HRESULT hr;

	// �ǂݍ��܂ꂽ�t�@�C����tga�`���Ȃ�
	if (strstr(_filePath, ".tga"))
	{
		hr = DirectX::LoadFromTGAFile(widePath, &metaData, image);
	}
	// �ǂݍ��܂ꂽ�t�@�C����psd�`���Ȃ�
	else if (strstr(_filePath, ".psd"))
	{
		std::string message = "psd�t�@�C���͑Ή����Ă��܂���\n" + std::string(_filePath);
		ImGuiDebugLog::Add(message);

		return false;
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
	hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metaData, &pTex->pSRV);

	if (FAILED(hr))
	{
		ImGuiDebugLog::Add("SRV�쐬�ŃG���[");
		return false;
	}
	else if (SUCCEEDED(hr))
	{
		pTex->width = static_cast<u_int>(metaData.width);
		pTex->height = static_cast<u_int>(metaData.height);
	}

	return true;
}

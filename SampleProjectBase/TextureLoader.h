#pragma once
#include "IAssetLoader.h"

class TextureLoader :
    public IAssetLoader
{
public:
	TextureLoader() {};
	~TextureLoader() {};

	/// <summary>
	/// �e�N�X�`�������[�h���ă��\�[�X�Ǘ��ɒǉ�����
	/// </summary>
	/// <param name="_texSetting">�e�N�X�`���f�[�^</param>
	/// <returns>�����������t���O</returns>
	bool Load(const char* _filePath) override;
};


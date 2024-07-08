#pragma once
#include "Model.h"
#include "Texture.h"

// ���f�������[�h���鏈���N���X
class ModelLoader
{
public:
	ModelLoader() {};
	~ModelLoader() {};

	/// <summary>
	/// ���f�������[�h���ă��\�[�X�Ǘ��ɒǉ�����
	/// </summary>
	/// <param name="_modelData">���f���f�[�^</param>
	/// <returns>�����������t���O</returns>
	static Model* Load(const ModelSettings& _modelData);
};

// �e�N�X�`�������[�h���鏈���N���X
class TextureLoader
{
public:
	struct TextureSetting
	{
		const char* pathName;	// �p�X��
		std::string setName;	// �z��ɃZ�b�g���閼�O
	};

public:
	TextureLoader() {};
	~TextureLoader() {};

	/// <summary>
	/// �e�N�X�`�������[�h���ă��\�[�X�Ǘ��ɒǉ�����
	/// </summary>
	/// <param name="_texSetting">�e�N�X�`���f�[�^</param>
	/// <returns>�����������t���O</returns>
	static Texture* Load(TextureSetting _texSetting);
};

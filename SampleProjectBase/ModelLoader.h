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



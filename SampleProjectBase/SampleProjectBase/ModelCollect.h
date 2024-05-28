#pragma once
#include "ResourceCollect_Base.h"
#include "Singleton_Base.h"

class Model;
class ModelSettings;

class ModelCollect :
	public  ResourceCollect_Base<Model>, public Singleton_Base<ModelCollect>
{
	friend class Singleton_Base<ModelCollect>;

public:
	bool Load(const ModelSettings& _settings);

	/// <summary>
	/// ���f���̃C���X�^���X��V�����m�ۂ��A�f�[�^���R�s�[���ēn��
	/// </summary>
	/// <param name="_modelName">�擾���郂�f�����</param>
	/// <returns>�����������f���̃C���X�^���X</returns>
	std::unique_ptr<Model> GetModel(std::string _modelName);
};


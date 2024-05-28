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
	/// モデルのインスタンスを新しく確保し、データをコピーして渡す
	/// </summary>
	/// <param name="_modelName">取得するモデル情報</param>
	/// <returns>生成したモデルのインスタンス</returns>
	std::unique_ptr<Model> GetModel(std::string _modelName);
};


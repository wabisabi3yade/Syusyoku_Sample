#pragma once
#include "Model.h"
#include "Texture.h"

// モデルをロードする処理クラス
class ModelLoader
{
public:
	ModelLoader() {};
	~ModelLoader() {};

	/// <summary>
	/// モデルをロードしてリソース管理に追加する
	/// </summary>
	/// <param name="_modelData">モデルデータ</param>
	/// <returns>成功したかフラグ</returns>
	static Model* Load(const ModelSettings& _modelData);
};



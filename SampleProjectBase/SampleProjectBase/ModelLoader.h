#pragma once
#include "Model.h"

// モデルをロードする処理クラス
class ModelLoader
{
public:
	ModelLoader(){};
	~ModelLoader(){};

	/// <summary>
	/// モデルをロードしてリソース管理に追加する
	/// </summary>
	/// <param name="_modelData">モデルデータ</param>
	/// <returns>成功したかフラグ</returns>
	static bool Load(const ModelSettings& _modelData);
};


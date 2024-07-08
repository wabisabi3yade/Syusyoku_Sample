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

// テクスチャをロードする処理クラス
class TextureLoader
{
public:
	struct TextureSetting
	{
		const char* pathName;	// パス名
		std::string setName;	// 配列にセットする名前
	};

public:
	TextureLoader() {};
	~TextureLoader() {};

	/// <summary>
	/// テクスチャをロードしてリソース管理に追加する
	/// </summary>
	/// <param name="_texSetting">テクスチャデータ</param>
	/// <returns>成功したかフラグ</returns>
	static Texture* Load(TextureSetting _texSetting);
};

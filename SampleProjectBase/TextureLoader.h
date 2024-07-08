#pragma once
#include "IAssetLoader.h"

class TextureLoader :
    public IAssetLoader
{
public:
	TextureLoader() {};
	~TextureLoader() {};

	/// <summary>
	/// テクスチャをロードしてリソース管理に追加する
	/// </summary>
	/// <param name="_texSetting">テクスチャデータ</param>
	/// <returns>成功したかフラグ</returns>
	bool Load(const char* _filePath) override;
};


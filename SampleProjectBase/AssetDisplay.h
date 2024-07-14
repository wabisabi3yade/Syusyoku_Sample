#pragma once
#include "AssetContacter.h"

// アセットのインポートされたものを表示する
class AssetDisplay : private AssetContacter
{
	AssetDisplay() {}
	~AssetDisplay() {}
public:

	/// @brief アセットの中身を描画
	static void Draw();
};


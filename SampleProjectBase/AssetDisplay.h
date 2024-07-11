#pragma once
#include "AssetContacter.h"

// アセットのインポートされたものを表示する
class AssetDisplay : private AssetContacter
{
	AssetDisplay() {}
	~AssetDisplay() {}
public:

	void Draw();
};


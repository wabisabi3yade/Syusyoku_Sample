#pragma once
#include "IAssetCreater.h"
#include "Material.h"

/// @brief マテリアル作成クラス
class MaterialCreater
{
public:
	MaterialCreater() {}
	~MaterialCreater() {}

	static Material& CraeteAsset(const std::string& _assetName);
};


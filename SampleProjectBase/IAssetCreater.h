#pragma once
#include "AssetContacter.h"

// アセットを作成するインターフェース
template<HashiTaku::AssetConcept T>
class IAssetCreater : public AssetContacter
{
public:
	IAssetCreater() {}
	virtual ~IAssetCreater() {}

	virtual T& CraeteAsset(const std::string& _assetName) = 0;
};


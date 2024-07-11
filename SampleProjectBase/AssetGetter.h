#pragma once
#include "AssetContacter.h"

#include "AssetCollection.h"

// 管理クラスからをアセットを取得するクラス
class AssetGetter : private AssetContacter
{
	AssetGetter() {}
	~AssetGetter() {}
public:

	template<HashiTaku::AssetType T>
	static T* GetAsset(const std::string& _assetName);
};

template<HashiTaku::AssetType T>
inline T* AssetGetter::GetAsset(const std::string& _assetName)
{
	return pAssetCollection->GetAsset(_assetName);
}

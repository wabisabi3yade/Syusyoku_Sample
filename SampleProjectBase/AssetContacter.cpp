#include "AssetContacter.h"

// アセット管理
#include "AssetCollection.h"

AssetCollection* AssetContacter::pAssetCollection = nullptr;

void AssetContacter::SetAssetCollection(AssetCollection& _assetCollection)
{
	pAssetCollection = &_assetCollection;
}

#include "AssetContacter.h"

// �A�Z�b�g�Ǘ�
#include "AssetCollection.h"

AssetCollection* AssetContacter::pAssetCollection = nullptr;

void AssetContacter::SetAssetCollection(AssetCollection& _assetCollection)
{
	pAssetCollection = &_assetCollection;
}

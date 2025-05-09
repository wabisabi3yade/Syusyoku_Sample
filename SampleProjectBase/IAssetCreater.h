#pragma once
#include "AssetContacter.h"

namespace HashiTaku
{
	// アセットを作成するインターフェース
	template<AssetConcept T>
	class IAssetCreater : public AssetContacter
	{
	public:
		IAssetCreater() {}
		virtual ~IAssetCreater() {}

		virtual T& CraeteAsset(const std::string& _assetName) = 0;
	};
}
#pragma once
#include "AssetConcept.h"

namespace HashiTaku
{
	class AssetCollection;

	// アセット管理をセットできるクラスを指定
	class MainApplication;

	// アセット管理にアクセスするクラスの基底クラス
	class AssetContacter
	{
		friend class MainApplication;

		/// @brief アセット管理クラスを設定
		/// @param _assetCollection アセット管理クラスの参照
		static void SetAssetCollection(AssetCollection& _assetCollection);
	protected:
		/// @brief アセット管理のクラス
		static AssetCollection* pAssetCollection;
	};
}
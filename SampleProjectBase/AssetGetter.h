#pragma once
#include "AssetContacter.h"

#include "AssetCollection.h"

// 管理クラスからをアセットを取得するクラス
class AssetGetter : private AssetContacter
{
	AssetGetter() {}
	~AssetGetter() {}
public:

	template<HashiTaku::AssetConcept T>
	static T* GetAsset(const std::string& _assetName);

	template<HashiTaku::AssetConcept T>
	static bool CheckImport(const std::string& _assetName);
};

template<HashiTaku::AssetConcept T>
inline T* AssetGetter::GetAsset(const std::string& _assetName)
{
	return pAssetCollection->GetAsset<T>(_assetName);
}

template<HashiTaku::AssetConcept T>
inline bool AssetGetter::CheckImport(const std::string& _assetName)
{
	return pAssetCollection->CheckImport<T>(_assetName);
}

namespace HashiTaku
{
	template<HashiTaku::AssetConcept T> T* LoadJsonAsset(const std::string& _s, const nlohmann::json& _j);

	template<HashiTaku::AssetConcept T>
	T* LoadJsonAsset(const std::string& _s, const nlohmann::json& _j)
	{
		if (!IsJsonContains(_j, _s)) return nullptr;

		if (!_j[_s].is_string())
		{
			HASHI_DEBUG_LOG(_s + "はstringではありません");
			return nullptr;
		}

		return AssetGetter::GetAsset<T>(_j[_s]);
	}
}

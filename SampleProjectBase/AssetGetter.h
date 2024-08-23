#pragma once
#include "AssetContacter.h"

#include "AssetCollection.h"

// �Ǘ��N���X������A�Z�b�g���擾����N���X
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
	return pAssetCollection->GetAsset<T>(_assetName);
}

namespace HashiTaku
{
	template<HashiTaku::AssetType T> T* LoadJsonAsset(const std::string& _s, const nlohmann::json& _j);

	template<HashiTaku::AssetType T>
	T* LoadJsonAsset(const std::string& _s, const nlohmann::json& _j)
	{
		if (!IsJsonContains(_j, _s)) return nullptr;

		if (!_j[_s].is_string())
		{
			HASHI_DEBUG_LOG(_s + "��string�ł͂���܂���");
			return nullptr;
		}

		return AssetGetter::GetAsset<T>(_j[_s]);
	}
}

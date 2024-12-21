#pragma once
#include "AssetContacter.h"

#include "AssetCollection.h"

namespace HashiTaku
{
	// 管理クラスからをアセットを取得するクラス
	class AssetGetter : private AssetContacter
	{
		AssetGetter() {}
		~AssetGetter() {}
	public:

		/// @brief 名前からアセットを取得
		/// @tparam T アセットの型
		/// @param _assetName アセット名
		/// @return アセットのポインタ
		template<AssetConcept T>
		static T* GetAsset(const std::string& _assetName);

		/// @brief アセットがインポートされているか確認
		/// @tparam T アセットの型
		/// @param _assetName アセット名前
		/// @return インポートしているか？
		template<AssetConcept T>
		static bool CheckImport(const std::string& _assetName);

		/*/// @brief ImGuiからコンボボックスでアセットを取得する
		/// @tparam T アセットの型
		/// @param _asset 代入したいアセットの変数
		/// @param _caption キャプション
		/// @return 変更したか？
		template<AssetConcept T>
		static bool ImGuiGetCombobox(T* _asset, const std::string& _caption);*/

		/// @brief ImGuiからコンボボックスでアセットを取得する
		/// @tparam T アセットの型
		/// @param _asset 代入したいアセットの変数
		/// @param _caption キャプション
		/// @param _currentName 現在選んでる
		/// @return 変更したか？
		template<AssetConcept T>
		static bool ImGuiGetCombobox(const std::string& _caption, std::string& _currentName);
	};

	template<AssetConcept T>
	inline T* AssetGetter::GetAsset(const std::string& _assetName)
	{
		return pAssetCollection->GetAsset<T>(_assetName);
	}

	template<AssetConcept T>
	inline bool AssetGetter::CheckImport(const std::string& _assetName)
	{
		return pAssetCollection->CheckImport<T>(_assetName);
	}

	template<AssetConcept T>
	inline bool AssetGetter::ImGuiGetCombobox(const std::string& _caption, std::string& _currentName)
	{
		bool isChange = false;

#ifdef EDIT
		AssetList& assetList = pAssetCollection->GetAssetList<T>();
		std::vector<const std::string*> assetNames;
		for (auto& asset : assetList)
		{
			assetNames.push_back(&asset.first);
		}

		isChange = ImGuiMethod::ComboBox(_caption, _currentName, assetNames);
#endif // EDIT

		return isChange;
	}

	template<AssetConcept T> T* LoadJsonAsset(const std::string& _s, const json& _j);

	template<AssetConcept T>
	T* LoadJsonAsset(const std::string& _s, const json& _j)
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
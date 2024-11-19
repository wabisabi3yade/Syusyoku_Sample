#pragma once
// アセットコンセプト
#include "AssetConcept.h"

// 扱うアセットの型
class Texture;
class Mesh_Group;
class SkeletalMesh;
class StaticMesh;
class Material;
class AnimationData;
class VisualEffect;
class BoneList;
class AnimationController;

// アセット管理にアクセスするクラス
class AssetLoader;
class AssetGetter;
class AssetDisplay;
class AssetSetter;
class AssetSaveLoader;

// アセットの管理配列
using AssetList = std::unordered_map<std::string, std::unique_ptr<Asset_Base>>;

// ゲーム内での利用するアセットを管理するクラス
// 管理クラスはユーザーからアクセスできてしまうとアセットを無造作に入れられたりする
// ので決めたクラスからしかアクセスできないようにする
class AssetCollection
{
	// アクセスできるクラス
	friend class AssetLoader;
	friend class AssetGetter;
	friend class AssetDisplay;
	friend class AssetSetter;
	friend class AssetSaveLoader;

	/// @brief テクスチャのアセットリスト
	AssetList textureAssets;

	/// @brief スタティック・スケルタルメッシュのアセットリスト
	AssetList modelAssets;

	/// @brief マテリアルのアセットリスト
	AssetList materialAssets;

	/// @brief エフェクトのアセットリスト
	AssetList vfxAssets;

	/// @brief アニメーションのアセットリスト
	AssetList animationAssets;

	/// @brief ボーンのアセットリスト
	AssetList boneAssets;

	/// @brief アニメーションコントローラー
	AssetList animControllerAssets;

	/// @brief アセットを追加する
	/// @tparam T アセットの型名
	/// @param _assetName アセットの名前
	/// @param _assetPtr アセットのユニークポインタ
	/// @return アセットのポインタ
	template<class T> T* SetAsset(const std::string& _assetName, std::unique_ptr<T> _assetPtr);

	/// @brief アセットを取得する
	/// @tparam T アセットの型名
	/// @param _assetName アセットの名前
	/// @return アセットのポインタ
	template<class T> T* GetAsset(const std::string& _assetName);

	/// @brief アセットを削除する
	/// @tparam T アセットの型名
	/// @param _assetName アセットの名前
	template<class T> void DeleteAsset(const std::string& _assetName);
public:
	AssetCollection() {}
	~AssetCollection() {}

	/// @brief アセットがインポートされているか確認
	/// @tparam T アセットの型名
	/// @param _assetName アセットの名前
	/// @return アセットがインポートしているかどうか
	template<class T> bool CheckImport(const std::string& _assetName);

private:
	/// @brief どの配列を取り出すか
	/// @tparam T アセットのタイプ
	/// @return アセットの配列
	template<class T> AssetList& GetAssetList();
};

/// @brief アセットがインポートされているか確認
/// @tparam T アセットの型名
/// @param _assetName アセットの名前
/// @return アセットがインポートしているかどうか
template<class T>
inline bool AssetCollection::CheckImport(const std::string& _assetName)
{
	// 対応した配列を取り出す
	AssetList& assetList = GetAssetList<T>();

	// 検索する
	auto itr = assetList.find(_assetName);

	// 最後だとインポートされていない
	return itr != assetList.end() ? true : false;
}

/// @brief アセットを追加する
/// @tparam T アセットの型名
/// @param _assetName アセットの名前
/// @param _assetPtr アセットのユニークポインタ
/// @return アセットのポインタ
template<class T>
inline T* AssetCollection::SetAsset(const std::string& _assetName, std::unique_ptr<T> _assetPtr)
{
	AssetList& assetList = GetAssetList<T>();

	// 同じアセットがインポートしてたならそのアセットを返す
	if (CheckImport<T>(_assetName))
		return static_cast<T*>(assetList[_assetName].get());

	Asset_Base& asset = static_cast<Asset_Base&>(*_assetPtr);
	asset.SetAssetName(_assetName);

	// 戻り値のポインタを取得
	T* pRetAsset = _assetPtr.get();
	assetList[_assetName] = std::move(_assetPtr);

	return pRetAsset;
}

/// @brief アセットを取得する
/// @tparam T アセットの型名
/// @param _assetName アセットの名前
/// @return アセットのポインタ
template<class T>
inline T* AssetCollection::GetAsset(const std::string& _assetName)
{
	AssetList& assetList = GetAssetList<T>();

	// 名前から探す
	auto itr = assetList.find(_assetName);
	if (itr == assetList.end())
	{
		std::string message = "ロードされていないので取得できません　" + _assetName;
		HASHI_DEBUG_LOG(message);
		return nullptr;
	}

	// ポインタをもらう
	Asset_Base* basePtr = itr->second.get();
	T* retPtr = static_cast<T*>(basePtr);

	return retPtr;	
}

template<class T>
inline void AssetCollection::DeleteAsset(const std::string& _assetName)
{
	AssetList& assetList = GetAssetList<T>();

	assetList.erase(_assetName);
}

/// @brief どの配列を取り出すか
/// @tparam T アセットのタイプ
/// @return アセットの配列
template<class T>
inline AssetList& AssetCollection::GetAssetList()
{
	// テクスチャ
	if constexpr (std::is_same<T, Texture>::value)
		return textureAssets;

	// スタティック・スケルタルメッシュメッシュ
	else if constexpr (std::is_same<T, Mesh_Group>::value || std::is_same<T, SkeletalMesh>::value ||
		std::is_same<T, StaticMesh>::value)
		return modelAssets;

	// マテリアル
	else if constexpr (std::is_same<T, Material>::value)
		return materialAssets;

	// エフェクト
	else if constexpr (std::is_same<T, VisualEffect>::value)
		return vfxAssets;

	// アニメーション
	else if constexpr (std::is_same<T, AnimationData>::value)
		return animationAssets;

	else if constexpr (std::is_same<T, BoneList>::value)
		return boneAssets;

	else if constexpr (std::is_same<T, AnimationController>::value)
		return animControllerAssets;

	assert(!"アセット配列取得で不正な型名です");
	return textureAssets;
}

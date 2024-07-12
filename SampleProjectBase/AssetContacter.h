#pragma once

class AssetCollection;

// アセット管理をセットできるクラスを指定
class MainApplication;

// アセットの型名
class Texture;
class StaticMesh;
class SkeletalMesh;
class Material;

// 橋本拓実の名前空間
namespace HashiTaku	
{
	// コンセプトで取得できるリソースの型を指定
	template<typename T>
	concept AssetType =
		std::is_same_v<T, Texture> || std::is_same_v<T, StaticMesh> ||
		std::is_same_v<T, SkeletalMesh> || std::is_same_v<T, Material>;
}

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
#pragma once
#include "AssetContacter.h"

// アセット情報をセーブロードするクラス
class AssetSaveLoader : public AssetContacter
{
	AssetSaveLoader() {}
	~AssetSaveLoader() {}

public:
	/// @brief アセットをセーブ
	/// @param _assetCollection アセットコレクション
	static void Save();

	/// @brief アセットをロード
	/// @param _assetCollection アセットコレクション 
	static void Load();

private:
	/// @brief セーブするファイルパスを取得
	/// @return ファイルパス
	static std::string SaveFilePath();

	// ロード時のアセット作成
	static void CreateTexture(const nlohmann::json& _loadData);
	static void CreateMesh(const nlohmann::json& _loadData);
	static void CreateMaterial(const nlohmann::json& _loadData);
	static void CreateVFX(const nlohmann::json& _loadData);
	static void CreateAnimaton(const nlohmann::json& _loadData);
	static void CreateAnimController(const nlohmann::json& _loadData);

	// アセットをロード
	static void LoadAsset(const nlohmann::json& _loadData);
};

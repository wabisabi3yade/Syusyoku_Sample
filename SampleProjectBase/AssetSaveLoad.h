#pragma once
#include "AssetContacter.h"

namespace HashiTaku
{
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
		static void CreateTexture(const json& _loadData);
		static void CreateMesh(const json& _loadData);
		static void CreateMaterial(const json& _loadData);
		static void CreateVFX(const json& _loadData);
		static void CreateAnimaton(const json& _loadData);
		static void CreateAnimController(const json& _loadData);

		// アセットをロード
		static void LoadAsset(const json& _loadData);
	};
}
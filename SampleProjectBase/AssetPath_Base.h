#pragma once
#include "Asset_Base.h"

namespace HashiTaku
{
	/// @brief パス名から作成するアセット基底
	class AssetPath_Base : public Asset_Base
	{
		/// @brief パス名
		std::string pathName;
	public:
		AssetPath_Base() : pathName("") {}
		virtual ~AssetPath_Base() {}

		// パス名をセット
		void SetPathName(const std::string& _pathName);

		// パス名を取得
		std::string GetPathName() const;

		/// @brief セーブする
		/// @param _sceneData セーブデータ
		nlohmann::json Save() override;

		/// @brief ロードする
		/// @param _sceneData ロードするデータ 
		void Load(const nlohmann::json& _data) override;
	};
}
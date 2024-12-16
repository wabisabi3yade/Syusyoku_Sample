#pragma once
#include "ISaveLoad.h"

namespace HashiTaku
{
	// アセットの基底クラス
	class Asset_Base : public ISaveLoad
	{
		/// @brief アセットの名前
		std::string assetName;

		/// @brief セーブするか？
		bool isSave;
	public:

		/// @brief コンストラクタ
		/// @param _type アセットの種類
		Asset_Base() : assetName(""), isSave(true) {};
		virtual ~Asset_Base() {};

		/// @brief 名前をセットする
		/// @param _name アセットの名前
		void SetAssetName(const std::string& _name);

		/// @brief 名前を取得する
		/// @return アセットの名前
		const std::string& GetAssetName() const;

		// セーブするか取得
		bool GetIsSave() const;

		/// @brief セーブする
		/// @return セーブデータ
		nlohmann::json Save() override;

		/// @brief ロードする
		/// @param _data ロードするデータ 
		void Load(const nlohmann::json& _data) override;

		/// @brief セーブしないようにする
		void SetIsntSave();
	};
}
#pragma once
#include "SaveJson.h"

namespace HashiTaku
{
	/// @brief セーブ・ロードをするインターフェース
	class ISaveLoad
	{
	public:
		ISaveLoad() {}
		virtual ~ISaveLoad() {}

		/// @brief セーブする
		/// @return セーブデータ
		virtual nlohmann::json Save() = 0;

		/// @brief ロードする
		/// @param _data ロードするデータ 
		virtual void Load(const nlohmann::json& _data) = 0;

		/*nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;*/
	};
}



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
		virtual json Save() = 0;

		/// @brief ロードする
		/// @param _data ロードするデータ 
		virtual void Load(const json& _data) = 0;

		/*json Save() override;
		void Load(const json& _data) override;*/
	};
}



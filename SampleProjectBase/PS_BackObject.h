#pragma once
#include "PixelShader.h"

namespace HashiTaku
{
	/// @brief 背景のオブジェクトのPS
	class PS_BackObject : public PixelShader
	{
	public:
		PS_BackObject();
		~PS_BackObject() {}

		/// @brief アセット初期化
		void InitAssets() override;

		/// @brief 一回のみの更新
		void OnceUpdateBuffer() override;
	};

}


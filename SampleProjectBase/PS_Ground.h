#pragma once
#include "PixelShader.h"

namespace HashiTaku
{
	/// @brief 地面描画で使用するピクセルシェーダー
	class PS_Ground : public PixelShader
	{

	public:
		PS_Ground();
		~PS_Ground() {}


		void OnceUpdateBuffer();
	};
}



#pragma once
#include "PixelShader.h"

namespace HashiTaku
{
	/// @brief トゥーンシェーダー
	class PS_Toon : public PixelShader
	{
	public:
		PS_Toon();
		~PS_Toon() {}

		void OnceUpdateBuffer() override;
	};
}
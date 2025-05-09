#pragma once
#include "PixelShader.h"

namespace HashiTaku
{
	/// @brief テクスチャの色だけのピクセルシェーダー
	class PS_TexColor : public PixelShader
	{
	public:
		PS_TexColor();
		~PS_TexColor() {}
	};
}
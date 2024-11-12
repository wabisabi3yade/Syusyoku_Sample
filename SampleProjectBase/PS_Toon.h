#pragma once
#include "PixelShader.h"

/// @brief トゥーンシェーダー
class PS_Toon : public PixelShader
{
public:
	PS_Toon();
	~PS_Toon() {}

	void OnceUpdateBuffer() override;
};


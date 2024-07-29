#pragma once
#include "PixelShader.h"

/// @brief Unlitのピクセルシェーダー
class PS_Unlit : public PixelShader
{
public:
	PS_Unlit();
	~PS_Unlit() {}

	void OnceUpdateBuffer() override;
};

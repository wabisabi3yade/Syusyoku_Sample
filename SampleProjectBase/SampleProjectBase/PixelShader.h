#pragma once
#include "Shader.h"

// ピクセルシェーダーにバッファを送ったりなどするクラス
class PixelShader : public Shader
{
	void MakeBuffer() override;

public:
	PixelShader();
	~PixelShader();
};


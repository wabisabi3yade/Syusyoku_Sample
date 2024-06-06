#pragma once
#include "Shader.h"

// 頂点シェーダーにバッファを送ったりなどするクラス
class VertexShader : public Shader
{
	void MakeBuffer() override;

public:
	VertexShader();
	~VertexShader();
};


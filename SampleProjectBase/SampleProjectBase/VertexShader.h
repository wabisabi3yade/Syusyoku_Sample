#pragma once
#include "Shader.h"

// 頂点シェーダーにバッファを送ったりなどするクラス
class VertexShader : public Shader
{
	std::unique_ptr<ID3D11VertexShader> pVxShader;	// 頂点シェーダー

	void MakeShader(const char* _pData, u_int _dataSize) override;
public:
	VertexShader();
	~VertexShader();

	// GPUにシェーダーを送る
	void Bind() override;
};


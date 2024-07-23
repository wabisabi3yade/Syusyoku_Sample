#pragma once
#include "Shader.h"

// ピクセルシェーダーのセットアップをするクラス
class PixelShader : public Shader
{
	ID3D11PixelShader* pPxShader;	// ピクセルシェーダー

	// ピクセルシェーダー作成
	void MakeShader(const char* _pData, u_int _dataSize) override;

public:
	PixelShader() : Shader(Shader::Type::Pixel), pPxShader(nullptr) {}
	~PixelShader();

	// GPUにシェーダーを送る
	void SetGPU() override;
};

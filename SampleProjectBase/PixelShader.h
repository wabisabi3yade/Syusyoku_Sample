#pragma once
#include "Shader.h"

namespace HashiTaku
{
	/// @brief ピクセルシェーダーのセットアップをするクラス
	class PixelShader : public Shader
	{
		/// @brief ピクセルシェーダー
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPxShader;

		// ピクセルシェーダー作成
		void MakeShader(const char* _pData, u_int _dataSize) override;

	public:
		PixelShader() : Shader(Shader::Type::Pixel) {}
		~PixelShader() {}

		// GPUにシェーダーを送る
		void SetGPU() override;
	};
}
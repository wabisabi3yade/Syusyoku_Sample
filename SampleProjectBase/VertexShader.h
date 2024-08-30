#pragma once
#include "Shader.h"

/// @brief 頂点シェーダーにバッファを送ったりなどするクラス
class VertexShader : public Shader
{
	/// @brief 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVxShader;
protected:

	/// @brief 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

	/// @brief 入力レイアウトを作成する
	virtual void MakeInputLayout(const char* _pData, u_int _dataSize);
public:
	VertexShader();
	~VertexShader();

	/// @brief GPUに情報を送る
	void SetGPU() override;

	void OnceUpdateBuffer() override {}
private:

	/// @brief 頂点シェーダー作成
	/// @param _pData シェーダー内容
	/// @param _dataSize データのサイズ
	void MakeShader(const char* _pData, u_int _dataSize) override;
};

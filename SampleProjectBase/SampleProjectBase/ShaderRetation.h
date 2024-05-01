#pragma once

// シェーダーリソースを保有するクラス
class ShaderRetation
{
	// 頂点シェーダー
	ID3D11VertexShader* pVertexShader = nullptr;
	// ピクセルシェーダー
	ID3D11PixelShader* pPixelShader = nullptr;
	// 入力レイアウトオブジェクト（VRAMに情報を入力するオブジェクト）
	ID3D11InputLayout* pInputLayout = nullptr;

public:
	ShaderRetation();
	~ShaderRetation();

	// 代入する
	void SetVertexShader(ID3D11VertexShader* _pSetVertex);	// 頂点シェーダー
	void SetPixelShader(ID3D11PixelShader* _pSetPixel);	// ピクセルシェーダー
	void SetInputLayout(ID3D11InputLayout* _pSetInput);	// 入力レイアウトオブジェクト

	// 取得する
	ID3D11VertexShader* GetVertexShader() { return pVertexShader; }
	ID3D11PixelShader* GetPixelShader() { return pPixelShader; };
	ID3D11InputLayout* GetInputLayout() { return pInputLayout; };
};


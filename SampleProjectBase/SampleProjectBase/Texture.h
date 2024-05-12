#pragma once

// テクスチャクラス
class Texture
{
	// シェーダーリソースビュー(SRV)
	// テクスチャ、バッファなどのリソースをシェーダーで参照可能な形式
	ID3D11ShaderResourceView* pSRV;

	// 画像(横・縦サイズ)
	u_int width;
	u_int height;

public:

	// ファイル形式を指定した形式に変更する
	static const wchar_t* ReplaceExtension(const std::wstring& _pathName, const char* ext);

	Texture();
	Texture(const char* _pathName, bool& _isSucess);
	// 準備する
	bool Setup(const char* _pathName);

	void Release();

	ID3D11ShaderResourceView* GetSRV() { return pSRV; }

	u_int GetWidth()const { return width; }	// 幅を取得
	u_int GetHeight()const { return height; }	// 高さを取得

	~Texture();
};


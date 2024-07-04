#pragma once

// テクスチャクラス
class Texture
{	bool isImported;	// モデル情報が入ったかどうか

	// 画像(横・縦サイズ)
	u_int width;
	u_int height;

	// シェーダーリソースビュー(SRV)
	// テクスチャ、バッファなどのリソースをシェーダーで参照可能な形式
	ID3D11ShaderResourceView* pSRV;
public:
	// ファイル形式を指定した形式に変更する
	static const wchar_t* ReplaceExtension(const std::wstring& _pathName, const char* ext);

	Texture();
	// 準備する
	bool Load(const char* _pathName);

	void Release();

	ID3D11ShaderResourceView* GetSRV()const { return pSRV; }

	u_int GetWidth()const { return width; }	// 幅を取得
	u_int GetHeight()const { return height; }	// 高さを取得

	~Texture();
};


#pragma once

// テクスチャクラス
class Texture
{

	// 画像(横・縦サイズ)
	u_int width;
	u_int height;

	// シェーダーリソースビュー(SRV)
	// テクスチャ、バッファなどのリソースをシェーダーで参照可能な形式
	ID3D11ShaderResourceView* pSRV;
public:

	Texture();
	// 準備する
	bool Load(const char* _pathName);

	void Release();

	ID3D11ShaderResourceView* GetSRV()const { return pSRV; }

	u_int GetWidth()const { return width; }	// 幅を取得
	u_int GetHeight()const { return height; }	// 高さを取得

	~Texture();
};


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

	// 準備する
	bool Setup(const char* _pathName);

public:
	Texture(const char* _pathName);
	~Texture();
};


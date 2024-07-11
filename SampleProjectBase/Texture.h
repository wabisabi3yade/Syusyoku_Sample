#pragma once
#include "Asset_Base.h"

/// @brief テクスチャクラス
class Texture : public Asset_Base
{
	friend class AssetLoader;

	/// @brief テクスチャ、バッファなどのリソースをシェーダーで参照可能な形式
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV;

	/// @brief 画像(横・縦サイズ)
	u_int width;
	u_int height;
public:
	Texture() : width(0), height(0) {}
	~Texture() {}

	/// @brief SRVの参照を取得
	/// @return SRVの参照
	ID3D11ShaderResourceView& GetSRV()const { return *pSRV.Get(); }

	/// @brief 幅を取得
	/// @return 画像の横幅(px)
	u_int GetWidth()const { return width; }

	/// @brief 高さを取得
	/// @return 画像の高さ(px)
	u_int GetHeight()const { return height; }
};

/// @brief テクスチャのNullオブジェクトクラス
class NullTexture : public Texture
{
public:
	NullTexture() {}
	~NullTexture() {}

	/// @brief 幅を取得
	/// @return 画像の横幅(px)
	u_int GetWidth()const { return 0; }

	/// @brief 高さを取得
	/// @return 画像の高さ(px)
	u_int GetHeight()const { return 0; }
};


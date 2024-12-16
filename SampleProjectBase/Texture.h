#pragma once
#include "AssetPath_Base.h"

namespace HashiTaku
{
	/// @brief テクスチャクラス
	class Texture : public AssetPath_Base
	{
		friend class AssetLoader;

	protected:
		/// @brief テクスチャ、バッファなどのリソースをシェーダーで参照可能な形式
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV;

		/// @brief 画像(横・縦サイズ)
		u_int width;
		u_int height;
	public:
		Texture() : width(0), height(0) {}
		~Texture() {}

		// SRVをセット
		virtual void SetSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pSRV);

		// 横幅をセット
		virtual void SetWidth(u_int _width);

		// 縦幅をセット
		virtual void SetHeight(u_int _height);

		/// @brief SRVの参照を取得
		/// @return SRVの参照
		virtual ID3D11ShaderResourceView& GetSRV()const { return *pSRV.Get(); }

		/// @brief 幅を取得
		/// @return 画像の横幅(px)
		virtual u_int GetWidth()const { return width; }

		/// @brief 高さを取得
		/// @return 画像の高さ(px)
		virtual u_int GetHeight()const { return height; }

		virtual bool GetIsSetSRV() const;
	};

	/// @brief テクスチャのNullオブジェクトクラス
	class NullTexture : public Texture
	{
	public:
		NullTexture() {}
		~NullTexture() {}

		// SRVをセット
		void SetSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pSRV) override {}

		// 横幅をセット
		void SetWidth(u_int _width) override {}

		// 縦幅をセット
		void SetHeight(u_int _height) override {}

		/// @brief 幅を取得
		/// @return 画像の横幅(px)
		u_int GetWidth()const override { return 0; }

		/// @brief 高さを取得
		/// @return 画像の高さ(px)
		u_int GetHeight()const override { return 0; }

		bool GetIsSetSRV() const override { return false; }
	};
}
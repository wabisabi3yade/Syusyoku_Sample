#pragma once
#include "Texture.h"

namespace HashiTaku
{
	/// @brief レンダーターゲットや深度テクスチャの基底
	class D3DTexture_Base : public Texture
	{
	protected:
		/// @brief テクスチャ
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

	public:
		D3DTexture_Base();
		virtual ~D3DTexture_Base() {}

	protected:
		/// @brief Description作成
		/// @param _format フォーマット
		/// @param _width 横幅
		/// @param _height 高さ
		/// @return 作成したDescription
		D3D11_TEXTURE2D_DESC MakeTexDesc(DXGI_FORMAT _format, UINT _width, UINT _height);

		/// @brief 各リソース作成
		/// @param _desc desc
		/// @param _pData データ
		/// @return 成功したか？
		virtual bool CreateResource(D3D11_TEXTURE2D_DESC& _desc, const void* _pData = nullptr);
	};

	/// @brief レンダーターゲットクラス
	class RenderTarget : public D3DTexture_Base
	{
		/// @brief  レンダーターゲットビュー
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTV;
	public:
		RenderTarget();
		~RenderTarget() {}

		/// @brief レンダーターゲット作成
		/// @param _format descのフォーマット
		/// @param _width 横幅
		/// @param _height 高さ
		/// @return 作成成功したか？
		bool Create(DXGI_FORMAT _format, UINT _width, UINT _height);

		/// @brief レンダーターゲットビューを取得
		/// @return レンダーターゲットビュー
		ID3D11RenderTargetView* GetView() const;

	private:
		/// @brief リソース作成
		/// @param _desc desc
		/// @param pData データ
		/// @return 成功したか？
		bool CreateResource(D3D11_TEXTURE2D_DESC& _desc, const void* pData = nullptr) override;
	};
}
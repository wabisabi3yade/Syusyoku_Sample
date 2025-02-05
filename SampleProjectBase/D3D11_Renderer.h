#pragma once
#include "RenderParam.h"
#include "IGetRenderResource.h"
#include "RenderTarget.h"
#include "DepthStencil.h"

namespace HashiTaku
{
	class BlendState;
	class Sampler;
	class SetUpPerspectiveProj;
	class SetUpViewTrans;

	constexpr u_int RASTERIZE_NUM(3);

	// Direct3D11の描画クラス
	class D3D11_Renderer : public IGetRenderResource
	{
	private:
		// 機能レベルの配列
		static const UINT FEATURE_LEVELS_NUM = 4;
		D3D_FEATURE_LEVEL pFeatureLevels[FEATURE_LEVELS_NUM] = {};
		// デバイス作成時に返される機能レベル
		D3D_FEATURE_LEVEL featureLevelsSupported;
		// デバイス＝DirectXの各種機能を作る
		Microsoft::WRL::ComPtr<ID3D11Device> pD3DDevice;
		// コンテキスト＝描画関連を司る機能
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
		// スワップチェイン＝ダブルバッファ機能
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
		// レンダーターゲット＝描画先を表す機能
		ID3D11RenderTargetView* pRenderTargetView;
		// 深度バッファ用テクスチャ
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilTexture;
		// 深度バッファ
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
		
		/// @brief 深度書き込み深度ステンシルステート
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthWriteState;

		/// @brief 深度書き込まない深度ステンシルステート
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pNoDepthWriteState;

		// ラスタライザ
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizerStates[RASTERIZE_NUM];

		std::vector<D3D11_VIEWPORT> viewPorts;	// ビューポート

		UINT backBufferNum = 3;
		UINT screenWidth = 0;
		UINT screenHeight = 0;

		std::unique_ptr<RenderParam> pRenderParam;	// 描画に必要な情報(定数バッファなど)
		std::unique_ptr<BlendState> pBlendState;	// ブレンドステート（半透明処理）のクラス
		std::unique_ptr<Sampler> pSampler;	// サンプラー
	public:
		D3D11_Renderer(HWND _hWnd);
		~D3D11_Renderer();

		void SetUpDraw();    // 描画処理の準備（クリアスクリーン）

		void Swap();    //バックバッファをフロントバッファ(画面)へ表示

		/// @brief カリングモードを設定
		/// @param _cullMode カリングモード
		void SetCullingMode(D3D11_CULL_MODE _cullMode);

		/// @brief 深度書き込みを切り替える
		/// @param _isWrite 書き込むようにするか？
		void SerDepthWrite(bool _isWrite);

		/// @brief レンダーターゲットをセット
		/// @param _viewPortId ビューポートのID
		/// @param _pRrenderTarget レンダーターゲット
		/// @param _pDepthStencils 深度ステンシル
		void SetRenderTerget(u_int _cnt, RenderTarget* _pRrenderTarget, DepthStencil* _pDepthStencil);

		/// @brief 元のレンダーターゲットに戻す
		void SetBaseRenderTarget();

		// 描画の情報を取得
		RenderParam& GetParameter() override;
		ID3D11Device* GetDevice()const override { return pD3DDevice.Get(); }
		ID3D11DeviceContext* GetDeviceContext() override { return pDeviceContext.Get(); }
		IDXGISwapChain* GetSwapChain() override;
		ID3D11DepthStencilView* GetDepthStencil();

		// ビューポートを取得（どのビューポートを指定）
		const D3D11_VIEWPORT& GetViewPort(u_int _slot) { return viewPorts[_slot]; }
		u_int GetViewPortNum() { return static_cast<u_int>(viewPorts.size()); }

		/// @brief ウィンドウの横幅を取得
		/// @return ウィンドウの横幅
		u_int GetWindowWidth() const;

		/// @brief ウィンドウの縦幅を取得
		/// @return ウィンドウの縦幅
		u_int GetWindowHeight() const;

	private:
		bool Init(HWND _hWnd);  // 初期化
		bool InitDeviceAndSwapChain(HWND _hWnd);    // デバイスとスワップチェインの作成
		bool InitBackBuffer();  // バックバッファの初期化

		/// @brief フルスクリーンにするか確認ん
		/// @param _hWnd ハンドル
		void CheckFullScreen(HWND _hWnd);

		void Release(); // 解放処理
	};
}
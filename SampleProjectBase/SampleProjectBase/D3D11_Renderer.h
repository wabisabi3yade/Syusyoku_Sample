#pragma once
#include "RenderParam.h"
#include "IGetRenderResource.h"

class ShaderCollection;
class BlendState;
class Sampler;
class SetUpPerspectiveProj;
class SetUpViewTrans;

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
	ID3D11Device* pD3DDevice = nullptr;
	// コンテキスト＝描画関連を司る機能
	ID3D11DeviceContext* pImmediateContext{ nullptr };
	// スワップチェイン＝ダブルバッファ機能
	IDXGISwapChain* pSwapChain = nullptr;
	// レンダーターゲット＝描画先を表す機能
	ID3D11RenderTargetView* pRenderTargetView{ nullptr };
	// 深度バッファ用テクスチャ
	ID3D11Texture2D* pDepthStencilTexture{ nullptr };
	// 深度バッファ
	ID3D11DepthStencilView* pDepthStencilView{ nullptr };

	std::vector<D3D11_VIEWPORT> viewPorts;	// ビューポート

	UINT backBufferNum = 3;
	UINT screenWidth = 0;
	UINT screenHeight = 0;

	std::unique_ptr<RenderParam> pRenderParam;	// 描画に必要な情報(定数バッファなど)
	std::unique_ptr<BlendState> pBlendState;	// ブレンドステート（半透明処理）のクラス
	std::unique_ptr<Sampler> pSampler;	// サンプラー

	bool Init(HWND _hWnd);  // 初期化
	bool InitDeviceAndSwapChain(HWND _hWnd);    // デバイスとスワップチェインの作成
	bool InitBackBuffer();  // バックバッファの初期化

	void Release(); // 解放処理

public:
	D3D11_Renderer(HWND _hWnd);
	~D3D11_Renderer();

	static DirectX::SimpleMath::Matrix GetWorldMtx(Transform _transform);

	void SetUpDraw();    // 描画処理の準備（クリアスクリーン）

	void Swap();    //バックバッファをフロントバッファ(画面)へ表示

	// 描画の情報を取得
	RenderParam& GetParameter() override;
	ID3D11Device* GetDevice()const override { return pD3DDevice; }
	ID3D11DeviceContext* GetDeviceContext() override { return pImmediateContext; }
	// ビューポートを取得（どのビューポートを指定）
	const D3D11_VIEWPORT& GetViewPort(u_int _slot) { return viewPorts[_slot]; }
	u_int GetViewPortNum() { return viewPorts.size(); }
};


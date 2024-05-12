#pragma once
#include "RenderParam.h"
#include "IGetRenderResource.h"

class ShaderRetation;
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
	// ビューポート
	D3D11_VIEWPORT viewPort[1];

	UINT backBufferNum = 3;
	UINT screenWidth = 0;
	UINT screenHeight = 0;

	// 描画に必要な情報(定数バッファなど)
	RenderParam renderParam;

	// ブレンドステート（半透明処理）のクラス
	BlendState* pBlendState {nullptr};

	// サンプラー
	Sampler* pSampler{ nullptr };

	// プロジェクション行列を準備するクラス
	std::unique_ptr<SetUpPerspectiveProj> pProjection;
	// ビュー変換行列を準備するクラス
	std::unique_ptr<SetUpViewTrans> pViewTransform;

	std::unique_ptr<ShaderRetation> pShaderRetation; // シェーダー管理

	bool Init(HWND _hWnd);  // 初期化
	bool InitDeviceAndSwapChain(HWND _hWnd);    // デバイスとスワップチェインの作成
	bool InitBackBuffer();  // バックバッファの初期化

	// シェーダーをコンパイルしてシェーダーオブジェクトを作成
    // →VRAMにシェーダーのマシン語データを送る
	bool CompileShader(const WCHAR* _pVsPath, const WCHAR* _pPsPath, ShaderRetation& _outShader);

	void Release(); // 解放処理

public:
	D3D11_Renderer(HWND _hWnd);
	~D3D11_Renderer();

	void SetUpDraw();    // 描画処理の準備（クリアスクリーン）

	void Swap();    //バックバッファをフロントバッファ(画面)へ表示

	// カメラからのビュー行列を受け取り、シェーダーに渡す準備
	void SetUpViewTransform(DirectX::XMMATRIX _viewMatrix);

	// 描画の情報を取得
	RenderParam& GetParameter() override { return renderParam; }

	ID3D11Device* GetDevice()const override { return pD3DDevice; }
	ID3D11DeviceContext* GetDeviceContext() override { return pImmediateContext; }

	ID3D11VertexShader* GetVertexShader();	// 頂点シェーダー取得
	ID3D11PixelShader* GetPixelShader();	// ピクセルシェーダー取得
};


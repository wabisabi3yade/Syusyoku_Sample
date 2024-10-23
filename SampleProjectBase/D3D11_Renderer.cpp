#include "D3D11_Renderer.h"
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include "ShaderCollection.h"
#include "BlendState.h"
#include "Sampler.h"
#include "SetUpPerspectiveProj.h"
#include "SetUpViewTrans.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

constexpr float DEFAULT_FOV = 45.0f;
constexpr float DEFAULT_NEARZ = 0.1f;
constexpr float DEFAULT_FARZ = 1000.0f;

D3D11_Renderer::D3D11_Renderer(HWND _hWnd)
{
	pRenderParam = std::make_unique<RenderParam>();

	bool isSuccess = Init(_hWnd);   // 初期化
	if (!isSuccess)
		HASHI_DEBUG_LOG("D3D11描画クラス初期化でエラー");

	// フルスクリーンするか確認
	CheckFullScreen(_hWnd);
}

RenderParam& D3D11_Renderer::GetParameter()
{
	RenderParam* ptr = pRenderParam.get();
	return *ptr;
}

IDXGISwapChain* D3D11_Renderer::GetSwapChain()
{
	return pSwapChain.Get();
}

ID3D11DepthStencilView* D3D11_Renderer::GetDepthStencil()
{
	return pDepthStencilView.Get();
}

bool D3D11_Renderer::Init(HWND _hWnd)
{
	bool isResult; // 初期化成功したか
	// Windowに合わせてスクリーンサイズ初期化
	RECT rc;
	GetClientRect(_hWnd, &rc);
	screenWidth = rc.right - rc.left;
	screenHeight = rc.bottom - rc.top;

	// 初期化
	isResult = InitDeviceAndSwapChain(_hWnd);
	if (!isResult) return false;

	isResult = InitBackBuffer();
	if (!isResult) return false;

	return true;
}

bool D3D11_Renderer::InitDeviceAndSwapChain(HWND _hWnd)
{
	// HRESULT型＝Windowsプログラムで関数の呼び出しが成功/失敗した結果を格納
	HRESULT hr;

	UINT flags = 0;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL pLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL level;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = backBufferNum;        // バックバッファの数
	sd.BufferDesc.Width = screenWidth;    // バックバッファの幅
	sd.BufferDesc.Height = screenHeight;    // バックバッファの高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // フォーマット
	sd.BufferDesc.RefreshRate.Numerator = 60;    // リフレッシュレート（分母）
	sd.BufferDesc.RefreshRate.Denominator = 1;    // リフレッシュレート（分子）
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // バックバッファの使用法
	sd.OutputWindow = _hWnd;        // 関連付けるウィンドウ
	sd.SampleDesc.Count = 1;            // マルチサンプル（アンチエイリアス）の数
	sd.SampleDesc.Quality = 0;            // マルチサンプル（アンチエイリアス）のクオリティ
	sd.Windowed = TRUE;        // ウィンドウモード（TRUEがウィンドウモード）
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;        // モード自動切り替え
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// デバイスと、コンテキスト、スワップチェインを作る関数を呼び出し
	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		pLevels,
		1,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pD3DDevice,
		&level,
		&pDeviceContext);

	if (FAILED(hr)) // ifで上の関数が失敗してないか判定
	{
		return false;
	}
	return true;
}


bool D3D11_Renderer::InitBackBuffer()
{
	HRESULT hr;

	// スワップ・チェインから最初のバック・バッファを取得する
	ID3D11Texture2D* pBackBuffer;  // バッファのアクセスに使うインターフェイス
	hr = pSwapChain->GetBuffer(
		0,                         // バック・バッファの番号
		__uuidof(ID3D11Texture2D), // バッファにアクセスするインターフェイス
		(LPVOID*)&pBackBuffer);    // バッファを受け取る変数
	if (FAILED(hr)) {

		return false;
	}

	//// バック・バッファの情報
	//D3D11_TEXTURE2D_DESC descBackBuffer;
	//pBackBuffer->GetDesc(&descBackBuffer);

	// バック・バッファの描画ターゲット・ビューを作る
	hr = pD3DDevice->CreateRenderTargetView(
		pBackBuffer,           // ビューでアクセスするリソース
		nullptr,               // 描画ターゲット・ビューの定義
		&pRenderTargetView); // 描画ターゲット・ビューを受け取る変数
	SAFE_RELEASE(pBackBuffer);  // 以降、バック・バッファは直接使わないので解放
	if (FAILED(hr))
	{
		return false;
	}

	// 深度ステンシルバッファを作成
	// ※深度バッファ（Zバッファ）→奥行を判定して前後関係を正しく描画できる
	D3D11_TEXTURE2D_DESC txDesc;
	ZeroMemory(&txDesc, sizeof(txDesc));
	txDesc.Width = screenWidth;
	txDesc.Height = screenHeight;
	txDesc.MipLevels = 1;
	txDesc.ArraySize = 1;
	txDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	txDesc.SampleDesc.Count = 1;
	txDesc.SampleDesc.Quality = 0;
	txDesc.Usage = D3D11_USAGE_DEFAULT;
	txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	txDesc.CPUAccessFlags = 0;
	txDesc.MiscFlags = 0;
	hr = pD3DDevice->CreateTexture2D(&txDesc, NULL, &pDepthStencilTexture);
	if (FAILED(hr))
		return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.Format = txDesc.Format;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsDesc.Texture2D.MipSlice = 0;
	hr = pD3DDevice->CreateDepthStencilView(pDepthStencilTexture.Get(), &dsDesc, &pDepthStencilView);
	if (FAILED(hr))
		return false;

	// 深度ステンシルバッファを作成
	// ※深度バッファ（Zバッファ）→奥行を判定して前後関係を正しく描画できる
	D3D11_TEXTURE2D_DESC d_txDesc;
	ZeroMemory(&d_txDesc, sizeof(d_txDesc));
	d_txDesc.Width = screenWidth;
	d_txDesc.Height = screenHeight;
	d_txDesc.MipLevels = 1;
	d_txDesc.ArraySize = 1;
	d_txDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d_txDesc.SampleDesc.Count = 1;
	d_txDesc.SampleDesc.Quality = 0;
	d_txDesc.Usage = D3D11_USAGE_DEFAULT;
	d_txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d_txDesc.CPUAccessFlags = 0;
	d_txDesc.MiscFlags = 0;
	hr = pD3DDevice->CreateTexture2D(&d_txDesc, NULL, &pDisableDepthStencilTexture);
	if (FAILED(hr))
		return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC d_dsDesc;
	ZeroMemory(&d_dsDesc, sizeof(d_dsDesc));
	d_dsDesc.Format = d_txDesc.Format;
	d_dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d_dsDesc.Texture2D.MipSlice = 0;
	hr = pD3DDevice->CreateDepthStencilView(pDisableDepthStencilTexture.Get(), &d_dsDesc, &pDisableDepthStencilView);
	if (FAILED(hr))
		return false;

	// ビューポートの設定
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;    // ビューポート領域の左上X座標。
	viewport.TopLeftY = 0.0f;    // ビューポート領域の左上Y座標。
	viewport.Width = static_cast<float>(screenWidth);  // ビューポート領域の幅
	viewport.Height = static_cast<float>(screenHeight);  // ビューポート領域の高さ
	viewport.MinDepth = 0.0f; // ビューポート領域の深度値の最小値
	viewport.MaxDepth = 1.0f; // ビューポート領域の深度値の最大値
	pDeviceContext->RSSetViewports(1, &viewport);
	viewPorts.push_back(viewport);	// 追加

	// ブレンドステート初期化
	pBlendState = std::make_unique<BlendState>();
	bool isResult = pBlendState->Init(*pD3DDevice.Get());
	if (!isResult)
		return false;
	// OMにブレンドステートオブジェクトを設定
	// OMは出力(Output)マネージャーのこと
	FLOAT BlendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	// RenderTargetが1つのときは基本的に考慮不要で0xffffffff
	pDeviceContext->OMSetBlendState(pBlendState->GetParaObject(), BlendFactor, 0xffffffff);

	// サンプラー初期化
	pSampler = std::make_unique<Sampler>();
	isResult = pSampler->Init(*pD3DDevice.Get());
	if (!isResult)
		return false;
	// サンプラー
	ID3D11SamplerState* sampler = pSampler->GetSampler();
	pDeviceContext->PSSetSamplers(0, 1, &sampler);

	// カリング設定
	D3D11_RASTERIZER_DESC rasterizer = {};
	D3D11_CULL_MODE cull[] = {
		D3D11_CULL_NONE,
		D3D11_CULL_FRONT,
		D3D11_CULL_BACK
	};
	rasterizer.FillMode = D3D11_FILL_SOLID;
	for (int rasIdx = 0; rasIdx < RASTERIZE_NUM; ++rasIdx)
	{
		rasterizer.CullMode = cull[rasIdx];
		hr = pD3DDevice->CreateRasterizerState(&rasterizer, &pRasterizerStates[rasIdx]);
		if (FAILED(hr))
			return false;
	}
	SetCullingMode(D3D11_CULL_BACK);

	return true;
}

void D3D11_Renderer::CheckFullScreen(HWND _hWnd)
{
	// フルスクリーンにするか
#ifndef _DEBUG

	int pushButton = MessageBoxA(_hWnd, "フルスクリーンで起動しますか？", "就職作品",
		MB_YESNO | MB_ICONQUESTION);

	if (pushButton == IDYES)
		pSwapChain->SetFullscreenState(TRUE, NULL);
#endif // _RELEASE
}

void D3D11_Renderer::Release()
{
	// デバイス・ステートのクリア
	if (pDeviceContext) pDeviceContext->ClearState();

	// スワップ チェインをウインドウ モードにする
	if (pSwapChain) pSwapChain->SetFullscreenState(FALSE, nullptr);

	// 取得したインターフェイスの開放
	SAFE_RELEASE(pRenderTargetView);
}


D3D11_Renderer::~D3D11_Renderer()
{
	Release();
}

void D3D11_Renderer::Swap()
{
	// バックバッファの表示（画面をすぐに更新）
	HRESULT hr = pSwapChain->Present(0, 0);
	if (FAILED(hr))
	{
		return;
	}
}

void D3D11_Renderer::SetCullingMode(D3D11_CULL_MODE _cullMode)
{
	switch (_cullMode)
	{
	case D3D11_CULL_NONE: pDeviceContext->RSSetState(pRasterizerStates[0].Get()); break;
	case D3D11_CULL_FRONT: pDeviceContext->RSSetState(pRasterizerStates[1].Get()); break;
	case D3D11_CULL_BACK: pDeviceContext->RSSetState(pRasterizerStates[2].Get()); break;
	default: assert(!"カリングモードが不正です");
	}
}

void D3D11_Renderer::SeStencil(bool _isStencil)
{
}

void D3D11_Renderer::SetUpDraw()
{
	if (!pDeviceContext || !pRenderTargetView) return;

	// 描画先のキャンバスと使用する深度バッファを指定する
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView.Get());

	float color[] = {0.0f ,0.0f ,1.0f, 0.0f};

	// 塗りつぶし
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	// 深度バッファをリセットする
	pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
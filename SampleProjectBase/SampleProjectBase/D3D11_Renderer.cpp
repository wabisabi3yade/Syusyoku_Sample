#include "D3D11_Renderer.h"
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include "ShaderRetation.h"
#include "BlendState.h"
#include "Sampler.h"
#include "SetUpPerspectiveProj.h"
#include "SetUpViewTrans.h"

D3D11_Renderer::D3D11_Renderer(HWND _hWnd)
{
	// クラス作成
	pShaderRetation = std::make_unique<ShaderRetation>();

	// 投影行列をバッファに入れる
	pProjection = std::make_unique<SetUpPerspectiveProj>();
	// ビュー変換行列を準備する
	pViewTransform = std::make_unique<SetUpViewTrans>();

	bool isSuccess = Init(_hWnd);   // 初期化
	if (!isSuccess)
		MessageError("D3D11描画クラス初期化でエラー");
}

bool D3D11_Renderer::Init(HWND _hWnd)
{
	bool isOk; // 初期化成功したか
	// Windowに合わせてスクリーンサイズ初期化
	RECT rc;
	GetClientRect(_hWnd, &rc);
	screenWidth = rc.right - rc.left;
	screenHeight = rc.bottom - rc.top;

	// 初期化
	isOk = InitDeviceAndSwapChain(_hWnd);
	if (!isOk) return false;

	isOk = InitBackBuffer();
	if (!isOk) return false;

	// シェーダーを作成する
	isOk = CompileShader(L"VertexShader.hlsl", L"PixelShader.hlsl", *pShaderRetation);
	if (!isOk) return false;

	isOk = renderParam.Init(GetDevice());
	if (!isOk) return false;

	// 投影行列をバッファに入れる
	isOk = pProjection->SetUpProjection(screenWidth, screenHeight, this);
	if (!isOk)
		return false;

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
		&pImmediateContext);

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
		//TRACE("InitBackBuffer g_pSwapChain->GetBuffer(%0x08x)n", hr);  // 失敗
		return false;
	}

	// バック・バッファの情報
	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc(&descBackBuffer);

	// バック・バッファの描画ターゲット・ビューを作る
	hr = pD3DDevice->CreateRenderTargetView(
		pBackBuffer,           // ビューでアクセスするリソース
		nullptr,               // 描画ターゲット・ビューの定義
		&pRenderTargetView); // 描画ターゲット・ビューを受け取る変数
	SAFE_RELEASE(pBackBuffer);  // 以降、バック・バッファは直接使わないので解放
	if (FAILED(hr)) {
		//TRACE("InitBackBuffer g_pD3DDevice->CreateRenderTargetView(%0x08x)n", hr);  // 失敗
		return false;
	}

	// ビューポートの設定
	viewPort[0].TopLeftX = 0.0f;    // ビューポート領域の左上X座標。
	viewPort[0].TopLeftY = 0.0f;    // ビューポート領域の左上Y座標。
	viewPort[0].Width = static_cast<float>(screenWidth);  // ビューポート領域の幅
	viewPort[0].Height = static_cast<float>(screenHeight);  // ビューポート領域の高さ
	viewPort[0].MinDepth = 0.0f; // ビューポート領域の深度値の最小値
	viewPort[0].MaxDepth = 1.0f; // ビューポート領域の深度値の最大値
	pImmediateContext->RSSetViewports(1, &viewPort[0]);

	// ブレンドステート初期化
	pBlendState = new BlendState(GetDevice());

	// サンプラー初期化
	pSampler = new Sampler();
	pSampler->Init(*GetDevice());

	return true;
}

void D3D11_Renderer::Release()
{
	// クラス解放
	CLASS_DELETE(pBlendState);
	CLASS_DELETE(pSampler);

	// デバイス・ステートのクリア
	if (pImmediateContext) pImmediateContext->ClearState();

	// スワップ チェインをウインドウ モードにする
	if (pSwapChain) pSwapChain->SetFullscreenState(FALSE, nullptr);

	// 取得したインターフェイスの開放
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pSwapChain);
	SAFE_RELEASE(pImmediateContext);
	SAFE_RELEASE(pD3DDevice);
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

void D3D11_Renderer::SetUpDraw()
{
	if (!pImmediateContext || !pRenderTargetView) return;

	pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

	float color[] = { 0.f, 0.f, 1.f, 0.f };
	
	pImmediateContext->ClearRenderTargetView(pRenderTargetView, color);
	ID3D11SamplerState* sampler = pSampler->GetSampler();
	pImmediateContext->PSSetSamplers(0, 1, &sampler);
	pImmediateContext->IASetInputLayout(pShaderRetation->GetInputLayout());
	pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pImmediateContext->VSSetShader(pShaderRetation->GetVertexShader(), nullptr, 0);
	pImmediateContext->PSSetShader(pShaderRetation->GetPixelShader(), nullptr, 0);

	// OMにブレンドステートオブジェクトを設定
	// OMは出力(Output)マネージャーのこと
	FLOAT BlendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	// RenderTargetが1つのときは基本的に考慮不要で0xffffffff
	pImmediateContext->OMSetBlendState(pBlendState->GetParaObject(), BlendFactor, 0xffffffff);
}

void D3D11_Renderer::SetUpViewTransform(DirectX::XMMATRIX _viewMatrix)
{
	// ビュー変換行列準備クラスに渡す
	pViewTransform->SetUpViewTransform(_viewMatrix, this);
}

ID3D11VertexShader* D3D11_Renderer::GetVertexShader()
{
	return pShaderRetation->GetVertexShader();
}

ID3D11PixelShader* D3D11_Renderer::GetPixelShader()
{
	return pShaderRetation->GetPixelShader();
}

bool D3D11_Renderer::CompileShader(const WCHAR* _pVsPath, const WCHAR* _pPsPath, ShaderRetation& _outShader)
{
	// ID3DBlob　→　メッシュの最適化と読み込み操作中に頂点、隣接性、
	// およびマテリアル情報を格納するデータバッファー
	ID3DBlob* pVsBlob = nullptr;
	ID3DBlob* errBlob = nullptr;
	// デバイスを取得
	auto pDevice = GetDevice();

	// シェーダーコンパイル
	HRESULT hr = D3DCompileFromFile(
		_pVsPath,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_4_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pVsBlob,
		&errBlob
	);
	if (FAILED(hr)) return false;

	// 頂点シェーダーオブジェクト作成
	ID3D11VertexShader* pVertexShader = nullptr;
	hr = pDevice->CreateVertexShader(
		pVsBlob->GetBufferPointer(),
		pVsBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	);
	if (FAILED(hr)) return false;

	// 入力レイアウトオブジェクト作成
	// →　頂点シェーダーにはどういったデータ構造の頂点データが渡ってくるかを定義した入力レイアウト
	ID3D11InputLayout* pInputLayout = nullptr;
	// Vertex構造体のメンバ変数のサイズを指定して、シェーダーの何の情報なのかを判別する
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	hr = pDevice->CreateInputLayout(
		layout,
		_countof(layout),
		pVsBlob->GetBufferPointer(),
		pVsBlob->GetBufferSize(),
		&pInputLayout
	);
	if (FAILED(hr)) return false;

	// ピクセルシェーダー作成
	ID3DBlob* pPsBlob = nullptr;
	hr = D3DCompileFromFile(
		_pPsPath,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_4_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pPsBlob,
		&errBlob
	);

	// ピクセルシェーダーオブジェクトを作成
	ID3D11PixelShader* pPixelShader = nullptr;
	hr = pDevice->CreatePixelShader(
		pPsBlob->GetBufferPointer(),
		pPsBlob->GetBufferSize(),
		nullptr,
		&pPixelShader
	);
	if (FAILED(hr)) return false;

	// 各オブジェクトをシェーダー保持クラスに代入する
	_outShader.SetVertexShader(pVertexShader);
	_outShader.SetPixelShader(pPixelShader);
	_outShader.SetInputLayout(pInputLayout);

	return true;
}

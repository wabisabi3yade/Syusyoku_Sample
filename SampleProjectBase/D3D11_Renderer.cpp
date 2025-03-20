#include "D3D11_Renderer.h"
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include "ShaderCollection.h"
#include "BlendState.h"
#include "Sampler.h"
#include "SetUpPerspectiveProj.h"
#include "SetUpViewTrans.h"

namespace HashiTaku
{
	// フルスクリーンクアッドで描画するシェーダー
	constexpr const char* DRAW_RT_VSNAME("VS_FullScreenQuad");
	constexpr const char* DRAW_RT_PSNAME("PS_FullScreenQuad");

	// モーションブラー
	constexpr const char* DRAW_MOTIONBLUR_PSNAME("PS_MotionBlur");

	// 輝度抽出・ブラー画像組み合わせのピクセルシェーダー
	constexpr const char* DRAW_LUMINANCE_PSNAME("PS_Luminance");
	constexpr const char* COMBINE_BLUR_PSNAME("PS_KawaseBlurCombine");
	constexpr const char* COMBINE_TEX_PSNAME("PS_TextureCombine");

	D3D11_Renderer::D3D11_Renderer() :
		pRenderTargetView(nullptr),
		pDrawRTVS(nullptr),
		pDrawRTPS(nullptr),
		pLuminancePS(nullptr),
		pCombineBlurPS(nullptr),
		pCombineTexPS(nullptr)
	{	
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

	RenderTargetCollection& D3D11_Renderer::GetRTCollection()
	{
		return *pRTCollection;
	}

	u_int D3D11_Renderer::GetWindowWidth() const
	{
		return screenWidth;
	}

	u_int D3D11_Renderer::GetWindowHeight() const
	{
		return screenHeight;
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

		// 描画パラメータを作成
		pRenderParam = std::make_unique<RenderParam>();

		// シェーダー管理初期化
		ShaderCollection::GetInstance()->Init();

		// レンダターゲット管理作成
		pRTCollection = std::make_unique<RenderTargetCollection>(*this);

		// レンダターゲット書き込み先ポリゴン作成
		CreateFullScreenMesh();

		// フルスクリーンするか確認
		CheckFullScreen(_hWnd);

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

		// 深度ステンシルステート
		// 深度書き込み無効のステンシルステート
		D3D11_DEPTH_STENCIL_DESC noDepthWriteDesc = {};
		noDepthWriteDesc.DepthEnable = TRUE;
		noDepthWriteDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // 書き込み無効
		noDepthWriteDesc.DepthFunc = D3D11_COMPARISON_LESS; // 通常の深度比較
		noDepthWriteDesc.StencilEnable = FALSE; // ステンシル無効
		pD3DDevice->CreateDepthStencilState(&noDepthWriteDesc, &pNoDepthWriteState);

		// 深度書き込み有効のステンシルステート
		D3D11_DEPTH_STENCIL_DESC depthWriteDesc = {};
		depthWriteDesc.DepthEnable = TRUE;
		depthWriteDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 書き込み有効
		depthWriteDesc.DepthFunc = D3D11_COMPARISON_LESS; // 通常の深度比較
		depthWriteDesc.StencilEnable = FALSE; // ステンシル無効
		pD3DDevice->CreateDepthStencilState(&depthWriteDesc, &pDepthWriteState);

		// 深度書き込むようにする
		SetDepthWrite(true);

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

		// ブレンドステート作成
		CreateBlendState();

		// なにもなしをセット
		SetBlendState(BlendState::BlendStateType::None);

		// サンプラー初期化
		pSampler = std::make_unique<Sampler>();
		bool isResult = pSampler->Init(*pD3DDevice.Get());
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

	void D3D11_Renderer::ApplyBloom()
	{
		using enum RenderTargetCollection::RenderTargetType;

		// 輝度抽出する
		pRTCollection->SetRenderTarget(Luminance, false);
		SetBlendState(BlendState::BlendStateType::None);

		// バッファをGPUに送る
		pLuminancePS->SetTexture(0, pRTCollection->GetRenderTarget(SceneDraw));
		pDrawRTVS->SetGPU();
		pLuminancePS->SetGPU();

		// メッシュを描画
		DrawFullScreenMesh();

		// 川瀬式ブルームをかける
		const RenderTarget* pLuminanceTex = pRTCollection->GetRenderTarget(Luminance);
		// 加算合成
		SetBlendState(BlendState::BlendStateType::Additive);

		// ダウンスケールして画像をぼかす
		for (u_int b_i = 0; b_i < KAWASE_BLUR_CNT; b_i++)
		{
			GausianBlur& gausianBlur = *kawaseBloomBlurs[b_i];

			// 次回へ渡す
			pLuminanceTex = &gausianBlur.ApplyBlur(*pLuminanceTex);

			// ブラー画像を生成し、
			pCombineBlurPS->SetTexture(b_i, pLuminanceTex);
		}
		
		// ブラー画像を組み合わせる
		pCombineBlurPS->SetGPU();
		pRTCollection->SetRenderTarget(CombineBlur, false);
		DrawFullScreenMesh();

		// シーン描画に組み合わせる
		pRTCollection->SetRenderTarget(SceneDraw, false);
		pCombineTexPS->SetTexture(0,pRTCollection->GetRenderTarget(SceneDraw));
		pCombineTexPS->SetTexture(1,pRTCollection->GetRenderTarget(CombineBlur));
		pCombineTexPS->SetGPU();
		DrawFullScreenMesh();
	}

	void D3D11_Renderer::ApplyMotionBlur()
	{
		using enum RenderTargetCollection::RenderTargetType;

		// モーションブラー画像を生成
		// レンダーターゲット
		pRTCollection->SetRenderTarget(MotionBlur, false);

		SetBlendState(BlendState::BlendStateType::None);

		// バッファをGPUに送る
		pMotionBlurPS->SetTexture(0, pRTCollection->GetRenderTarget(SceneDraw));
		pMotionBlurPS->SetTexture(1, pRTCollection->GetRenderTarget(MotionVector));
		pDrawRTVS->SetGPU();
		pMotionBlurPS->SetGPU();
		// メッシュを描画
		DrawFullScreenMesh();

		//// シーン描画に組み合わせる
		//pRTCollection->SetRenderTarget(SceneDraw, false);
		//pCombineTexPS->SetTexture(0, pRTCollection->GetRenderTarget(SceneDraw));
		//pCombineTexPS->SetTexture(1, pRTCollection->GetRenderTarget(MotionBlur));
		//pCombineTexPS->SetGPU();
		//DrawFullScreenMesh();
	}

	void D3D11_Renderer::CreateFullScreenMesh()
	{
		// 画面全体に描画するポリゴン
		pDrawRTMesh = std::make_unique<PlaneMesh>();

		// 頂点座標を変更する
		std::array<DXSimp::Vector3, 4> vertexPosList;
		vertexPosList[0] = DXSimp::Vector3(-1.0f, 1.0f, 0.0f);
		vertexPosList[1] = DXSimp::Vector3(1.0f, 1.0f, 0.0f);
		vertexPosList[2] = DXSimp::Vector3(-1.0f, -1.0f, 0.0f);
		vertexPosList[3] = DXSimp::Vector3(1.0f, -1.0f, 0.0f);
		pDrawRTMesh->SetVertexPos(vertexPosList);

		// 描画する頂点
		auto* shCol = ShaderCollection::GetInstance();
		pDrawRTVS = shCol->GetVertexShader(DRAW_RT_VSNAME);
		pDrawRTPS = shCol->GetPixelShader(DRAW_RT_PSNAME);
		pLuminancePS = shCol->GetPixelShader(DRAW_LUMINANCE_PSNAME);
		pCombineBlurPS = shCol->GetPixelShader(COMBINE_BLUR_PSNAME);
		pCombineTexPS = shCol->GetPixelShader(COMBINE_TEX_PSNAME);
		pMotionBlurPS = shCol->GetPixelShader(DRAW_MOTIONBLUR_PSNAME);

		// ブラークラスを生成
		// ダウンスケールの倍率
		u_int downScale = 1;
		for (u_int b_i = 0; b_i < KAWASE_BLUR_CNT; b_i++)
		{
			downScale *= 2;	// 次のダウンスケールを求める

			// レンダーターゲットのサイズを計算
			u_int rtWidth = screenWidth / downScale;
			u_int rtHeight = screenHeight / downScale;

			kawaseBloomBlurs[b_i] = std::make_unique<GausianBlur>(rtWidth, rtHeight);
		}
	}

	void D3D11_Renderer::CreateBlendState()
	{
		// ブレンドステートの種類分作成する
		u_int typeCnt = static_cast<u_int>(BlendState::BlendStateType::MaxNum);
		for (u_int bt_i = 0; bt_i < typeCnt; bt_i++)
		{
			BlendState::BlendStateType type = 
				static_cast<BlendState::BlendStateType>(bt_i);
			std::unique_ptr<BlendState> pCreateState = std::make_unique<BlendState>();

			// 作成
			bool isSuccess = pCreateState->Init(*pD3DDevice.Get(), type);
			if (!isSuccess) continue;

			blendStateList[bt_i] = std::move(pCreateState);
		}
	}

	void D3D11_Renderer::CheckFullScreen(HWND _hWnd)
	{
		// フルスクリーンにするか
#ifndef _DEBUG

		int pushButton = MessageBoxA(_hWnd, "フルスクリーンで起動しますか？", "Duel Knight",
			MB_YESNO | MB_ICONQUESTION);

		if (pushButton == IDYES)
		{
			pSwapChain->SetFullscreenState(TRUE, NULL);	// フルスクリーン
			SetCursor(NULL);	// カーソルを消す
		}
			

#endif // _DEBUG
	}

	void D3D11_Renderer::Release()
	{
		// デバイス・ステートのクリア
		if (pDeviceContext) pDeviceContext->ClearState();

		// スワップ チェインをウインドウ モードにする
		if (pSwapChain) pSwapChain->SetFullscreenState(FALSE, nullptr);

		ShaderCollection::Delete();

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

	void D3D11_Renderer::SetDepthWrite(bool _isWrite)
	{
		if (_isWrite)
		{
			pDeviceContext->OMSetDepthStencilState(pDepthWriteState.Get(), 0);
		}
		else
		{
			pDeviceContext->OMSetDepthStencilState(pNoDepthWriteState.Get(), 0);
		}
	}

	void D3D11_Renderer::SetBlendState(BlendState::BlendStateType _setBlendState)
	{
		// OMは出力(Output)マネージャーのこと
		FLOAT BlendFactor[4] = { 1.f, 1.f, 1.f, 1.f };

		// セットするブレンドステートを取得
		ID3D11BlendState* pSetBlendState = 
			blendStateList[static_cast<u_int>(_setBlendState)]->GetResourceObject();

		// RenderTargetが1つのときは基本的に考慮不要で0xffffffff
		pDeviceContext->OMSetBlendState(pSetBlendState, BlendFactor, 0xffffffff);
	}

	void D3D11_Renderer::SetRenderTerget(u_int _cnt, RenderTarget* _pRrenderTarget, DepthStencil& _depthStencil)
	{
		SetRenderTerget(_cnt, _pRrenderTarget, _depthStencil.GetView());
	}

	void D3D11_Renderer::SetRenderTerget(u_int _cnt, RenderTarget* _pRrenderTarget, ID3D11DepthStencilView* _pDepthStencilView)
	{
		static ID3D11RenderTargetView* rtvs[1];
		rtvs[0] = _pRrenderTarget->GetView();

		// レンダーターゲット設定
		pDeviceContext->OMSetRenderTargets(
			_cnt,
			rtvs,
			_pDepthStencilView ?
			_pDepthStencilView : nullptr
		);

		D3D11_VIEWPORT& vp = viewPorts[0];

		// ビューポート設定
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		vp.Width = static_cast<float>(_pRrenderTarget->GetWidth());
		vp.Height = static_cast<float>(_pRrenderTarget->GetHeight());
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		pDeviceContext->RSSetViewports(1, &vp);
	}

	void D3D11_Renderer::SetBaseRenderTarget()
	{
		pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView.Get());

		D3D11_VIEWPORT& vp = viewPorts[0];

		// ビューポート設定
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		vp.Width = static_cast<float>(GetWindowWidth());
		vp.Height = static_cast<float>(GetWindowHeight());
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		pDeviceContext->RSSetViewports(1, &vp);
	}

	void D3D11_Renderer::RenderFullScreenQuad()
	{
		using enum RenderTargetCollection::RenderTargetType;

		// シーン描画びブルーム処理をかける
		ApplyBloom();

		// モーションブラー
		ApplyMotionBlur();

		// スクリーンに四角形を描画
		SetBaseRenderTarget();
		SetBlendState(BlendState::BlendStateType::None);
		pDeviceContext->IASetPrimitiveTopology(pDrawRTMesh->GetTopology());

		// バッファをGPUに送る
		pDrawRTPS->SetTexture(0, pRTCollection->GetRenderTarget(SceneDraw));
		pDrawRTVS->SetGPU();
		pDrawRTPS->SetGPU();

		// メッシュを描画
		DrawFullScreenMesh();
	}

	void D3D11_Renderer::DrawFullScreenMesh()
	{
		// バッファをGPUに送る
		pDeviceContext->IASetPrimitiveTopology(pDrawRTMesh->GetTopology());
		pDrawRTMesh->GetVertexBuffer().SetGPU();
		pDrawRTMesh->GetIndexBuffer().SetGPU();

		pDeviceContext->DrawIndexed(
			pDrawRTMesh->GetIndexNum(),
			0,
			0
		);
	}

	void D3D11_Renderer::SetUpDraw()
	{
		if (!pDeviceContext || !pRenderTargetView) return;

		// 描画先のキャンバスと使用する深度バッファを指定する
		pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView.Get());

		float color[] = { 0.0f ,0.0f ,0.0f, 0.0f };

		// 塗りつぶし
		pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

		// 深度バッファをリセットする
		pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// レンダーターゲットをクリアする
		pRTCollection->Clear();
	}
}
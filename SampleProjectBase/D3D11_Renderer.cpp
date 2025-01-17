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
	constexpr float DEFAULT_FOV = 45.0f;
	constexpr float DEFAULT_NEARZ = 0.1f;
	constexpr float DEFAULT_FARZ = 1000.0f;

	D3D11_Renderer::D3D11_Renderer(HWND _hWnd)
	{
		pRenderParam = std::make_unique<RenderParam>();

		bool isSuccess = Init(_hWnd);   // ������
		if (!isSuccess)
			HASHI_DEBUG_LOG("D3D11�`��N���X�������ŃG���[");

		// �t���X�N���[�����邩�m�F
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
		bool isResult; // ����������������
		// Window�ɍ��킹�ăX�N���[���T�C�Y������
		RECT rc;
		GetClientRect(_hWnd, &rc);
		screenWidth = rc.right - rc.left;
		screenHeight = rc.bottom - rc.top;

		// ������
		isResult = InitDeviceAndSwapChain(_hWnd);
		if (!isResult) return false;

		isResult = InitBackBuffer();
		if (!isResult) return false;

		return true;
	}

	bool D3D11_Renderer::InitDeviceAndSwapChain(HWND _hWnd)
	{
		// HRESULT�^��Windows�v���O�����Ŋ֐��̌Ăяo��������/���s�������ʂ��i�[
		HRESULT hr;

		UINT flags = 0;
#ifdef _DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL pLevels[] = { D3D_FEATURE_LEVEL_11_0 };
		D3D_FEATURE_LEVEL level;

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = backBufferNum;        // �o�b�N�o�b�t�@�̐�
		sd.BufferDesc.Width = screenWidth;    // �o�b�N�o�b�t�@�̕�
		sd.BufferDesc.Height = screenHeight;    // �o�b�N�o�b�t�@�̍���
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // �t�H�[�}�b�g
		sd.BufferDesc.RefreshRate.Numerator = 60;    // ���t���b�V�����[�g�i����j
		sd.BufferDesc.RefreshRate.Denominator = 1;    // ���t���b�V�����[�g�i���q�j
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // �o�b�N�o�b�t�@�̎g�p�@
		sd.OutputWindow = _hWnd;        // �֘A�t����E�B���h�E
		sd.SampleDesc.Count = 1;            // �}���`�T���v���i�A���`�G�C���A�X�j�̐�
		sd.SampleDesc.Quality = 0;            // �}���`�T���v���i�A���`�G�C���A�X�j�̃N�I���e�B
		sd.Windowed = TRUE;        // �E�B���h�E���[�h�iTRUE���E�B���h�E���[�h�j
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;        // ���[�h�����؂�ւ�
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// �f�o�C�X�ƁA�R���e�L�X�g�A�X���b�v�`�F�C�������֐����Ăяo��
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

		if (FAILED(hr)) // if�ŏ�̊֐������s���ĂȂ�������
		{
			return false;
		}
		return true;
	}


	bool D3D11_Renderer::InitBackBuffer()
	{
		HRESULT hr;

		// �X���b�v�E�`�F�C������ŏ��̃o�b�N�E�o�b�t�@���擾����
		ID3D11Texture2D* pBackBuffer;  // �o�b�t�@�̃A�N�Z�X�Ɏg���C���^�[�t�F�C�X
		hr = pSwapChain->GetBuffer(
			0,                         // �o�b�N�E�o�b�t�@�̔ԍ�
			__uuidof(ID3D11Texture2D), // �o�b�t�@�ɃA�N�Z�X����C���^�[�t�F�C�X
			(LPVOID*)&pBackBuffer);    // �o�b�t�@���󂯎��ϐ�
		if (FAILED(hr)) {

			return false;
		}

		//// �o�b�N�E�o�b�t�@�̏��
		//D3D11_TEXTURE2D_DESC descBackBuffer;
		//pBackBuffer->GetDesc(&descBackBuffer);

		// �o�b�N�E�o�b�t�@�̕`��^�[�Q�b�g�E�r���[�����
		hr = pD3DDevice->CreateRenderTargetView(
			pBackBuffer,           // �r���[�ŃA�N�Z�X���郊�\�[�X
			nullptr,               // �`��^�[�Q�b�g�E�r���[�̒�`
			&pRenderTargetView); // �`��^�[�Q�b�g�E�r���[���󂯎��ϐ�
		SAFE_RELEASE(pBackBuffer);  // �ȍ~�A�o�b�N�E�o�b�t�@�͒��ڎg��Ȃ��̂ŉ��
		if (FAILED(hr))
		{
			return false;
		}

		// �[�x�X�e���V���o�b�t�@���쐬
		// ���[�x�o�b�t�@�iZ�o�b�t�@�j�����s�𔻒肵�đO��֌W�𐳂����`��ł���
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

		// �[�x�X�e���V���X�e�[�g
		// �[�x�������ݖ����̃X�e���V���X�e�[�g
		D3D11_DEPTH_STENCIL_DESC noDepthWriteDesc = {};
		noDepthWriteDesc.DepthEnable = TRUE;
		noDepthWriteDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // �������ݖ���
		noDepthWriteDesc.DepthFunc = D3D11_COMPARISON_LESS; // �ʏ�̐[�x��r
		noDepthWriteDesc.StencilEnable = FALSE; // �X�e���V������
		pD3DDevice->CreateDepthStencilState(&noDepthWriteDesc, &pNoDepthWriteState);

		// �[�x�������ݗL���̃X�e���V���X�e�[�g
		D3D11_DEPTH_STENCIL_DESC depthWriteDesc = {};
		depthWriteDesc.DepthEnable = TRUE;
		depthWriteDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // �������ݗL��
		depthWriteDesc.DepthFunc = D3D11_COMPARISON_LESS; // �ʏ�̐[�x��r
		depthWriteDesc.StencilEnable = FALSE; // �X�e���V������
		pD3DDevice->CreateDepthStencilState(&depthWriteDesc, &pDepthWriteState);

		// �[�x�������ނ悤�ɂ���
		SerDepthWrite(true);

		// �r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0.0f;    // �r���[�|�[�g�̈�̍���X���W�B
		viewport.TopLeftY = 0.0f;    // �r���[�|�[�g�̈�̍���Y���W�B
		viewport.Width = static_cast<float>(screenWidth);  // �r���[�|�[�g�̈�̕�
		viewport.Height = static_cast<float>(screenHeight);  // �r���[�|�[�g�̈�̍���
		viewport.MinDepth = 0.0f; // �r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
		viewport.MaxDepth = 1.0f; // �r���[�|�[�g�̈�̐[�x�l�̍ő�l
		pDeviceContext->RSSetViewports(1, &viewport);
		viewPorts.push_back(viewport);	// �ǉ�

		// �u�����h�X�e�[�g������
		pBlendState = std::make_unique<BlendState>();
		bool isResult = pBlendState->Init(*pD3DDevice.Get());
		if (!isResult)
			return false;
		// OM�Ƀu�����h�X�e�[�g�I�u�W�F�N�g��ݒ�
		// OM�͏o��(Output)�}�l�[�W���[�̂���
		FLOAT BlendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
		// RenderTarget��1�̂Ƃ��͊�{�I�ɍl���s�v��0xffffffff
		pDeviceContext->OMSetBlendState(pBlendState->GetParaObject(), BlendFactor, 0xffffffff);

		// �T���v���[������
		pSampler = std::make_unique<Sampler>();
		isResult = pSampler->Init(*pD3DDevice.Get());
		if (!isResult)
			return false;
		// �T���v���[
		ID3D11SamplerState* sampler = pSampler->GetSampler();
		pDeviceContext->PSSetSamplers(0, 1, &sampler);

		// �J�����O�ݒ�
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
		// �t���X�N���[���ɂ��邩
#ifndef _DEBUG

		int pushButton = MessageBoxA(_hWnd, "�t���X�N���[���ŋN�����܂����H", "Duel Knight",
			MB_YESNO | MB_ICONQUESTION);

		if (pushButton == IDYES)
			pSwapChain->SetFullscreenState(TRUE, NULL);
#endif // _DEBUG
	}

	void D3D11_Renderer::Release()
	{
		// �f�o�C�X�E�X�e�[�g�̃N���A
		if (pDeviceContext) pDeviceContext->ClearState();

		// �X���b�v �`�F�C�����E�C���h�E ���[�h�ɂ���
		if (pSwapChain) pSwapChain->SetFullscreenState(FALSE, nullptr);

		// �擾�����C���^�[�t�F�C�X�̊J��
		SAFE_RELEASE(pRenderTargetView);
	}


	D3D11_Renderer::~D3D11_Renderer()
	{
		Release();
	}

	void D3D11_Renderer::Swap()
	{
		// �o�b�N�o�b�t�@�̕\���i��ʂ������ɍX�V�j
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
		default: assert(!"�J�����O���[�h���s���ł�");
		}
	}

	void D3D11_Renderer::SerDepthWrite(bool _isWrite)
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

	void D3D11_Renderer::SetRenderTerget(u_int _cnt, RenderTarget* _pRrenderTarget, DepthStencil* _pDepthStencil)
	{
		static ID3D11RenderTargetView* rtvs[1];
		rtvs[0] = _pRrenderTarget->GetView();

		// �����_�[�^�[�Q�b�g�ݒ�
		pDeviceContext->OMSetRenderTargets(
			_cnt,
			rtvs,
			_pDepthStencil ?
			_pDepthStencil->GetView() : nullptr
		);

		D3D11_VIEWPORT& vp = viewPorts[0];

		// �r���[�|�[�g�ݒ�
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

		// �r���[�|�[�g�ݒ�
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		vp.Width = static_cast<float>(GetWindowWidth());
		vp.Height = static_cast<float>(GetWindowHeight());
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		pDeviceContext->RSSetViewports(1, &vp);
	}

	void D3D11_Renderer::SetUpDraw()
	{
		if (!pDeviceContext || !pRenderTargetView) return;

		// �`���̃L�����o�X�Ǝg�p����[�x�o�b�t�@���w�肷��
		pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView.Get());

		float color[] = { 0.0f ,0.0f ,0.0f, 0.0f };

		// �h��Ԃ�
		pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);

		// �[�x�o�b�t�@�����Z�b�g����
		pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}
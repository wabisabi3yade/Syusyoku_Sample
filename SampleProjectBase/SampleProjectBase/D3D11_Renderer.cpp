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
	// �N���X�쐬
	pShaderRetation = std::make_unique<ShaderRetation>();

	// ���e�s����o�b�t�@�ɓ����
	pProjection = std::make_unique<SetUpPerspectiveProj>();
	// �r���[�ϊ��s�����������
	pViewTransform = std::make_unique<SetUpViewTrans>();

	bool isSuccess = Init(_hWnd);   // ������
	if (!isSuccess)
		MessageError("D3D11�`��N���X�������ŃG���[");
}

bool D3D11_Renderer::Init(HWND _hWnd)
{
	bool isOk; // ����������������
	// Window�ɍ��킹�ăX�N���[���T�C�Y������
	RECT rc;
	GetClientRect(_hWnd, &rc);
	screenWidth = rc.right - rc.left;
	screenHeight = rc.bottom - rc.top;

	// ������
	isOk = InitDeviceAndSwapChain(_hWnd);
	if (!isOk) return false;

	isOk = InitBackBuffer();
	if (!isOk) return false;

	// �V�F�[�_�[���쐬����
	isOk = CompileShader(L"VertexShader.hlsl", L"PixelShader.hlsl", *pShaderRetation);
	if (!isOk) return false;

	isOk = renderParam.Init(GetDevice());
	if (!isOk) return false;

	// ���e�s����o�b�t�@�ɓ����
	isOk = pProjection->SetUpProjection(screenWidth, screenHeight, this);
	if (!isOk)
		return false;

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
		&pImmediateContext);

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
		//TRACE("InitBackBuffer g_pSwapChain->GetBuffer(%0x08x)n", hr);  // ���s
		return false;
	}

	// �o�b�N�E�o�b�t�@�̏��
	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc(&descBackBuffer);

	// �o�b�N�E�o�b�t�@�̕`��^�[�Q�b�g�E�r���[�����
	hr = pD3DDevice->CreateRenderTargetView(
		pBackBuffer,           // �r���[�ŃA�N�Z�X���郊�\�[�X
		nullptr,               // �`��^�[�Q�b�g�E�r���[�̒�`
		&pRenderTargetView); // �`��^�[�Q�b�g�E�r���[���󂯎��ϐ�
	SAFE_RELEASE(pBackBuffer);  // �ȍ~�A�o�b�N�E�o�b�t�@�͒��ڎg��Ȃ��̂ŉ��
	if (FAILED(hr)) {
		//TRACE("InitBackBuffer g_pD3DDevice->CreateRenderTargetView(%0x08x)n", hr);  // ���s
		return false;
	}

	// �r���[�|�[�g�̐ݒ�
	viewPort[0].TopLeftX = 0.0f;    // �r���[�|�[�g�̈�̍���X���W�B
	viewPort[0].TopLeftY = 0.0f;    // �r���[�|�[�g�̈�̍���Y���W�B
	viewPort[0].Width = static_cast<float>(screenWidth);  // �r���[�|�[�g�̈�̕�
	viewPort[0].Height = static_cast<float>(screenHeight);  // �r���[�|�[�g�̈�̍���
	viewPort[0].MinDepth = 0.0f; // �r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
	viewPort[0].MaxDepth = 1.0f; // �r���[�|�[�g�̈�̐[�x�l�̍ő�l
	pImmediateContext->RSSetViewports(1, &viewPort[0]);

	// �u�����h�X�e�[�g������
	pBlendState = new BlendState(GetDevice());

	// �T���v���[������
	pSampler = new Sampler();
	pSampler->Init(*GetDevice());

	return true;
}

void D3D11_Renderer::Release()
{
	// �N���X���
	CLASS_DELETE(pBlendState);
	CLASS_DELETE(pSampler);

	// �f�o�C�X�E�X�e�[�g�̃N���A
	if (pImmediateContext) pImmediateContext->ClearState();

	// �X���b�v �`�F�C�����E�C���h�E ���[�h�ɂ���
	if (pSwapChain) pSwapChain->SetFullscreenState(FALSE, nullptr);

	// �擾�����C���^�[�t�F�C�X�̊J��
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
	// �o�b�N�o�b�t�@�̕\���i��ʂ������ɍX�V�j
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

	// OM�Ƀu�����h�X�e�[�g�I�u�W�F�N�g��ݒ�
	// OM�͏o��(Output)�}�l�[�W���[�̂���
	FLOAT BlendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	// RenderTarget��1�̂Ƃ��͊�{�I�ɍl���s�v��0xffffffff
	pImmediateContext->OMSetBlendState(pBlendState->GetParaObject(), BlendFactor, 0xffffffff);
}

void D3D11_Renderer::SetUpViewTransform(DirectX::XMMATRIX _viewMatrix)
{
	// �r���[�ϊ��s�񏀔��N���X�ɓn��
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
	// ID3DBlob�@���@���b�V���̍œK���Ɠǂݍ��ݑ��쒆�ɒ��_�A�אڐ��A
	// ����у}�e���A�������i�[����f�[�^�o�b�t�@�[
	ID3DBlob* pVsBlob = nullptr;
	ID3DBlob* errBlob = nullptr;
	// �f�o�C�X���擾
	auto pDevice = GetDevice();

	// �V�F�[�_�[�R���p�C��
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

	// ���_�V�F�[�_�[�I�u�W�F�N�g�쐬
	ID3D11VertexShader* pVertexShader = nullptr;
	hr = pDevice->CreateVertexShader(
		pVsBlob->GetBufferPointer(),
		pVsBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	);
	if (FAILED(hr)) return false;

	// ���̓��C�A�E�g�I�u�W�F�N�g�쐬
	// ���@���_�V�F�[�_�[�ɂ͂ǂ��������f�[�^�\���̒��_�f�[�^���n���Ă��邩���`�������̓��C�A�E�g
	ID3D11InputLayout* pInputLayout = nullptr;
	// Vertex�\���̂̃����o�ϐ��̃T�C�Y���w�肵�āA�V�F�[�_�[�̉��̏��Ȃ̂��𔻕ʂ���
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

	// �s�N�Z���V�F�[�_�[�쐬
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

	// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g���쐬
	ID3D11PixelShader* pPixelShader = nullptr;
	hr = pDevice->CreatePixelShader(
		pPsBlob->GetBufferPointer(),
		pPsBlob->GetBufferSize(),
		nullptr,
		&pPixelShader
	);
	if (FAILED(hr)) return false;

	// �e�I�u�W�F�N�g���V�F�[�_�[�ێ��N���X�ɑ������
	_outShader.SetVertexShader(pVertexShader);
	_outShader.SetPixelShader(pPixelShader);
	_outShader.SetInputLayout(pInputLayout);

	return true;
}

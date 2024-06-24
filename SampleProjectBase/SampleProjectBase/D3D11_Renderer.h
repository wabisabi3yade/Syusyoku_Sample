#pragma once
#include "RenderParam.h"
#include "IGetRenderResource.h"

class ShaderCollection;
class BlendState;
class Sampler;
class SetUpPerspectiveProj;
class SetUpViewTrans;

// Direct3D11�̕`��N���X
class D3D11_Renderer : public IGetRenderResource
{
private:
	// �@�\���x���̔z��
	static const UINT FEATURE_LEVELS_NUM = 4;
	D3D_FEATURE_LEVEL pFeatureLevels[FEATURE_LEVELS_NUM] = {};
	// �f�o�C�X�쐬���ɕԂ����@�\���x��
	D3D_FEATURE_LEVEL featureLevelsSupported;
	// �f�o�C�X��DirectX�̊e��@�\�����
	ID3D11Device* pD3DDevice = nullptr;
	// �R���e�L�X�g���`��֘A���i��@�\
	ID3D11DeviceContext* pImmediateContext{ nullptr };
	// �X���b�v�`�F�C�����_�u���o�b�t�@�@�\
	IDXGISwapChain* pSwapChain = nullptr;
	// �����_�[�^�[�Q�b�g���`����\���@�\
	ID3D11RenderTargetView* pRenderTargetView{ nullptr };
	// �[�x�o�b�t�@�p�e�N�X�`��
	ID3D11Texture2D* pDepthStencilTexture{ nullptr };
	// �[�x�o�b�t�@
	ID3D11DepthStencilView* pDepthStencilView{ nullptr };

	std::vector<D3D11_VIEWPORT> viewPorts;	// �r���[�|�[�g

	UINT backBufferNum = 3;
	UINT screenWidth = 0;
	UINT screenHeight = 0;

	std::unique_ptr<RenderParam> pRenderParam;	// �`��ɕK�v�ȏ��(�萔�o�b�t�@�Ȃ�)
	std::unique_ptr<BlendState> pBlendState;	// �u�����h�X�e�[�g�i�����������j�̃N���X
	std::unique_ptr<Sampler> pSampler;	// �T���v���[

	bool Init(HWND _hWnd);  // ������
	bool InitDeviceAndSwapChain(HWND _hWnd);    // �f�o�C�X�ƃX���b�v�`�F�C���̍쐬
	bool InitBackBuffer();  // �o�b�N�o�b�t�@�̏�����

	void Release(); // �������

public:
	D3D11_Renderer(HWND _hWnd);
	~D3D11_Renderer();

	static DirectX::SimpleMath::Matrix GetWorldMtx(Transform _transform);

	void SetUpDraw();    // �`�揈���̏����i�N���A�X�N���[���j

	void Swap();    //�o�b�N�o�b�t�@���t�����g�o�b�t�@(���)�֕\��

	// �`��̏����擾
	RenderParam& GetParameter() override;
	ID3D11Device* GetDevice()const override { return pD3DDevice; }
	ID3D11DeviceContext* GetDeviceContext() override { return pImmediateContext; }
	// �r���[�|�[�g���擾�i�ǂ̃r���[�|�[�g���w��j
	const D3D11_VIEWPORT& GetViewPort(u_int _slot) { return viewPorts[_slot]; }
	u_int GetViewPortNum() { return viewPorts.size(); }
};


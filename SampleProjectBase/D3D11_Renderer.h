#pragma once
#include "RenderParam.h"
#include "IGetRenderResource.h"
#include "RenderTarget.h"
#include "DepthStencil.h"

class BlendState;
class Sampler;
class SetUpPerspectiveProj;
class SetUpViewTrans;

constexpr u_int RASTERIZE_NUM(3);

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
	Microsoft::WRL::ComPtr<ID3D11Device> pD3DDevice;
	// �R���e�L�X�g���`��֘A���i��@�\
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	// �X���b�v�`�F�C�����_�u���o�b�t�@�@�\
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	// �����_�[�^�[�Q�b�g���`����\���@�\
	ID3D11RenderTargetView* pRenderTargetView;
	// �[�x�o�b�t�@�p�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilTexture;
	// �[�x�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	// ���X�^���C�U
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizerStates[RASTERIZE_NUM];

	std::vector<D3D11_VIEWPORT> viewPorts;	// �r���[�|�[�g

	UINT backBufferNum = 3;
	UINT screenWidth = 0;
	UINT screenHeight = 0;

	std::unique_ptr<RenderParam> pRenderParam;	// �`��ɕK�v�ȏ��(�萔�o�b�t�@�Ȃ�)
	std::unique_ptr<BlendState> pBlendState;	// �u�����h�X�e�[�g�i�����������j�̃N���X
	std::unique_ptr<Sampler> pSampler;	// �T���v���[
public:
	D3D11_Renderer(HWND _hWnd);
	~D3D11_Renderer();

	void SetUpDraw();    // �`�揈���̏����i�N���A�X�N���[���j

	void Swap();    //�o�b�N�o�b�t�@���t�����g�o�b�t�@(���)�֕\��

	/// @brief �J�����O���[�h��ݒ�
	/// @param _cullMode �J�����O���[�h
	void SetCullingMode(D3D11_CULL_MODE _cullMode);

	/// @brief �����_�[�^�[�Q�b�g���Z�b�g
	/// @param _viewPortId �r���[�|�[�g��ID
	/// @param _pRrenderTarget �����_�[�^�[�Q�b�g
	/// @param _pDepthStencils �[�x�X�e���V��
	void SetRenderTerget(u_int _cnt, RenderTarget* _pRrenderTarget, DepthStencil* _pDepthStencil);

	// �`��̏����擾
	RenderParam& GetParameter() override;
	ID3D11Device* GetDevice()const override { return pD3DDevice.Get(); }
	ID3D11DeviceContext* GetDeviceContext() override { return pDeviceContext.Get(); }
	IDXGISwapChain* GetSwapChain() override;
	ID3D11DepthStencilView* GetDepthStencil();

	// �r���[�|�[�g���擾�i�ǂ̃r���[�|�[�g���w��j
	const D3D11_VIEWPORT& GetViewPort(u_int _slot) { return viewPorts[_slot]; }
	u_int GetViewPortNum() { return static_cast<u_int>(viewPorts.size()); }

	/// @brief �E�B���h�E�̉������擾
	/// @return �E�B���h�E�̉���
	u_int GetWindowWidth() const;

	/// @brief �E�B���h�E�̏c�����擾
	/// @return �E�B���h�E�̏c��
	u_int GetWindowHeight() const;

private:
	bool Init(HWND _hWnd);  // ������
	bool InitDeviceAndSwapChain(HWND _hWnd);    // �f�o�C�X�ƃX���b�v�`�F�C���̍쐬
	bool InitBackBuffer();  // �o�b�N�o�b�t�@�̏�����

	/// @brief �t���X�N���[���ɂ��邩�m�F��
	/// @param _hWnd �n���h��
	void CheckFullScreen(HWND _hWnd);

	void Release(); // �������
};


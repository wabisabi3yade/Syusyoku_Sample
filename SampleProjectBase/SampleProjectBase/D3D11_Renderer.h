#pragma once
#include "RenderParam.h"
#include "IGetRenderResource.h"

class ShaderRetation;
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
	// �r���[�|�[�g
	D3D11_VIEWPORT viewPort[1];

	UINT backBufferNum = 3;
	UINT screenWidth = 0;
	UINT screenHeight = 0;

	// �`��ɕK�v�ȏ��(�萔�o�b�t�@�Ȃ�)
	RenderParam renderParam;

	// �u�����h�X�e�[�g�i�����������j�̃N���X
	BlendState* pBlendState {nullptr};

	// �T���v���[
	Sampler* pSampler{ nullptr };

	// �v���W�F�N�V�����s�����������N���X
	std::unique_ptr<SetUpPerspectiveProj> pProjection;
	// �r���[�ϊ��s�����������N���X
	std::unique_ptr<SetUpViewTrans> pViewTransform;

	std::unique_ptr<ShaderRetation> pShaderRetation; // �V�F�[�_�[�Ǘ�

	bool Init(HWND _hWnd);  // ������
	bool InitDeviceAndSwapChain(HWND _hWnd);    // �f�o�C�X�ƃX���b�v�`�F�C���̍쐬
	bool InitBackBuffer();  // �o�b�N�o�b�t�@�̏�����

	// �V�F�[�_�[���R���p�C�����ăV�F�[�_�[�I�u�W�F�N�g���쐬
    // ��VRAM�ɃV�F�[�_�[�̃}�V����f�[�^�𑗂�
	bool CompileShader(const WCHAR* _pVsPath, const WCHAR* _pPsPath, ShaderRetation& _outShader);

	void Release(); // �������

public:
	D3D11_Renderer(HWND _hWnd);
	~D3D11_Renderer();

	void SetUpDraw();    // �`�揈���̏����i�N���A�X�N���[���j

	void Swap();    //�o�b�N�o�b�t�@���t�����g�o�b�t�@(���)�֕\��

	// �J��������̃r���[�s����󂯎��A�V�F�[�_�[�ɓn������
	void SetUpViewTransform(DirectX::XMMATRIX _viewMatrix);

	// �`��̏����擾
	RenderParam& GetParameter() override { return renderParam; }

	ID3D11Device* GetDevice()const override { return pD3DDevice; }
	ID3D11DeviceContext* GetDeviceContext() override { return pImmediateContext; }

	ID3D11VertexShader* GetVertexShader();	// ���_�V�F�[�_�[�擾
	ID3D11PixelShader* GetPixelShader();	// �s�N�Z���V�F�[�_�[�擾
};


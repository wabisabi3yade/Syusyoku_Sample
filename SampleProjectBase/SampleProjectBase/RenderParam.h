#pragma once

// �`��ɕK�v�ȃp�����[�^�N���X
class RenderParam
{
public:
	struct WVP	// ���[���h�E�r���[�E�v���W�F�N�V����	
	{
		DirectX::SimpleMath::Matrix world;	// ���[���h�ϊ��s��
		DirectX::SimpleMath::Matrix view;	// �r���[�ϊ�
		DirectX::SimpleMath::Matrix projection;	// �v���W�F�N�V����
	};

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;	// �C���v�b�g���C�A�E�g
	WVP wvp;	// ���[���h�s��
public:
	RenderParam() : pInputLayout(nullptr) {};
	~RenderParam() {};

	// �C���v�b�g���C�A�E�g���擾
	ID3D11InputLayout& GetInputLayout();
	const WVP& GetWVP() { return wvp; }	// WVP���擾

	// �C���v�b�g���C�A�E�g����
	void SetInputLayout(ID3D11InputLayout* _inputLayout) { pInputLayout = _inputLayout; }
	void SetView(DirectX::SimpleMath::Matrix _view) { wvp.view = _view; }
	void SetProjection(DirectX::SimpleMath::Matrix _proj) { wvp.projection = _proj; }
};


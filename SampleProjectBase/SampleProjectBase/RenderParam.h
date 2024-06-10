#pragma once

// �`��ɕK�v�ȃp�����[�^�N���X
class RenderParam
{
public:
	struct WorldMatrix
	{
		DirectX::SimpleMath::Matrix world;	// ���[���h���W
		DirectX::SimpleMath::Matrix view;	// �r���[�ϊ�
		DirectX::SimpleMath::Matrix projection;	// �v���W�F�N�V����
	};


private:
	ID3D11InputLayout* pInputLayout;	// �C���v�b�g���C�A�E�g
	WorldMatrix worldMatrix;	// ���[���h�s��
public:
	RenderParam() {};
	~RenderParam();

	// �C���v�b�g���C�A�E�g���擾
	ID3D11InputLayout& GetInputLayout();
	const WorldMatrix& GetWorldMatrix() { return worldMatrix; }

	// �C���v�b�g���C�A�E�g����
	void SetInputLayout(ID3D11InputLayout* _inputLayout) { pInputLayout = _inputLayout; }
	void SetView(DirectX::SimpleMath::Matrix _view) { worldMatrix.view = _view; }
	void SetProjection(DirectX::SimpleMath::Matrix _proj) { worldMatrix.projection = _proj; }
};


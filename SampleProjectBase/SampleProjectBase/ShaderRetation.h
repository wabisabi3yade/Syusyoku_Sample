#pragma once

// �V�F�[�_�[���\�[�X��ۗL����N���X
class ShaderRetation
{
	// ���_�V�F�[�_�[
	ID3D11VertexShader* pVertexShader = nullptr;
	// �s�N�Z���V�F�[�_�[
	ID3D11PixelShader* pPixelShader = nullptr;
	// ���̓��C�A�E�g�I�u�W�F�N�g�iVRAM�ɏ�����͂���I�u�W�F�N�g�j
	ID3D11InputLayout* pInputLayout = nullptr;

public:
	ShaderRetation();
	~ShaderRetation();

	// �������
	void SetVertexShader(ID3D11VertexShader* _pSetVertex);	// ���_�V�F�[�_�[
	void SetPixelShader(ID3D11PixelShader* _pSetPixel);	// �s�N�Z���V�F�[�_�[
	void SetInputLayout(ID3D11InputLayout* _pSetInput);	// ���̓��C�A�E�g�I�u�W�F�N�g

	// �擾����
	ID3D11VertexShader* GetVertexShader() { return pVertexShader; }
	ID3D11PixelShader* GetPixelShader() { return pPixelShader; };
	ID3D11InputLayout* GetInputLayout() { return pInputLayout; };
};


#pragma once
#include "Shader.h"

// ���_�V�F�[�_�[�Ƀo�b�t�@�𑗂�����Ȃǂ���N���X
class VertexShader : public Shader
{
	std::unique_ptr<ID3D11VertexShader> pVxShader;	// ���_�V�F�[�_�[

	void MakeShader(const char* _pData, u_int _dataSize) override;
public:
	VertexShader();
	~VertexShader();

	// GPU�ɃV�F�[�_�[�𑗂�
	void Bind() override;
};


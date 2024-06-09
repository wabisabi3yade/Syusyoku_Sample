#pragma once
#include "Shader.h"

// �s�N�Z���V�F�[�_�[�̃Z�b�g�A�b�v������N���X
class PixelShader : public Shader
{
	std::unique_ptr<ID3D11PixelShader> pPxShader;	// �s�N�Z���V�F�[�_�[

	// �s�N�Z���V�F�[�_�[�쐬
	void MakeShader(const char* _pData, u_int _dataSize) override;

public:
	PixelShader() : Shader(Shader::Type::Pixel) {}
	~PixelShader() {};

	// GPU�ɃV�F�[�_�[�𑗂�
	void Bind() override;
};


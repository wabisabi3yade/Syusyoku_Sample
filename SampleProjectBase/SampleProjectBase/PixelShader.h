#pragma once
#include "Shader.h"

// �s�N�Z���V�F�[�_�[�Ƀo�b�t�@�𑗂�����Ȃǂ���N���X
class PixelShader : public Shader
{
	void MakeBuffer() override;

public:
	PixelShader();
	~PixelShader();
};


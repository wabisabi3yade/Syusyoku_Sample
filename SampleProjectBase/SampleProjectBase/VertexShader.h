#pragma once
#include "Shader.h"

// ���_�V�F�[�_�[�Ƀo�b�t�@�𑗂�����Ȃǂ���N���X
class VertexShader : public Shader
{
	void MakeBuffer() override;

public:
	VertexShader();
	~VertexShader();
};


#pragma once

#include "VertexShader.h"

class VS_SkinnedAnimation : public VertexShader
{
	// �A�j���[�V�����p�ɃC���v�b�g���C�A�E�g���쐬���� 
	void MakeInputLayout(const char* _pData, u_int _dataSize) override;
public:
	VS_SkinnedAnimation();
	~VS_SkinnedAnimation() {}
};


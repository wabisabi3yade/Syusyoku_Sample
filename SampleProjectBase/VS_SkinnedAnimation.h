#pragma once

#include "VertexShader.h"

class VS_SkinnedAnimation : public VertexShader
{
	// アニメーション用にインプットレイアウトを作成する 
	void MakeInputLayout(const char* _pData, u_int _dataSize) override;
public:
	VS_SkinnedAnimation();
	~VS_SkinnedAnimation() {}
};


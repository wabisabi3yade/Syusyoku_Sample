#pragma once
#include "PixelShader.h"

/// @brief �g�D�[���V�F�[�_�[
class PS_Toon : public PixelShader
{
public:
	PS_Toon();
	~PS_Toon() {}

	void OnceUpdateBuffer() override;
};


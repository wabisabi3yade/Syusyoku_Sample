#pragma once
#include "PixelShader.h"

/// @brief Unlit�̃s�N�Z���V�F�[�_�[
class PS_Unlit : public PixelShader
{
public:
	PS_Unlit();
	~PS_Unlit() {}

	void OnceUpdateBuffer() override;
};

#pragma once
#include "PixelShader.h"

namespace HashiTaku
{
	/// @brief �n�ʕ`��Ŏg�p����s�N�Z���V�F�[�_�[
	class PS_Ground : public PixelShader
	{

	public:
		PS_Ground();
		~PS_Ground() {}


		void OnceUpdateBuffer();
	};
}



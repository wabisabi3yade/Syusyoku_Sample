#pragma once
#include "PixelShader.h"

namespace HashiTaku
{
	/// @brief Unlit�̃s�N�Z���V�F�[�_�[
	class PS_Unlit : public PixelShader
	{
	public:
		PS_Unlit();
		~PS_Unlit() {}

		void OnceUpdateBuffer() override;
	};
}
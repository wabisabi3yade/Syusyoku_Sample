#pragma once
#include "VertexShader.h"

namespace HashiTaku
{
	/// @brief �n�ʕ`��̒��_�V�F�[�_
	class VS_Ground : public VertexShader
	{

	public:
		VS_Ground();
		~VS_Ground() {}

		void OnceUpdateBuffer() override;
	};

}


#pragma once
#include "VertexShader.h"

namespace HashiTaku
{
	// �O���[�V�F�[�f�B���O�̒��_�V�F�[�_�[�N���X
	class VS_Gouraud : public VertexShader
	{
	public:
		VS_Gouraud();
		~VS_Gouraud() {}

		void OnceUpdateBuffer() override;
	};
}
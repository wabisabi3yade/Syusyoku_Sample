#pragma once
#include "SingleMesh.h"

namespace HashiTaku
{
	// �L���[�u���b�V���N���X
	class Cube : public SingleMesh
	{
		/// @brief �L���[�u���쐬
		void Make();
	public:

		Cube();
		~Cube() {};
	};
}
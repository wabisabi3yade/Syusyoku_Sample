#pragma once
#include "SingleMesh.h"

namespace HashiTaku
{
	/// @brief ���C���[�t���[���̃L���[�u
	class DebugCube : public SingleMesh
	{
		/// @brief  �L���[�u���쐬
		void Make();
	public:
		DebugCube();
		~DebugCube() {}
	};
}
#pragma once
#include "SingleMesh.h"

namespace HashiTaku
{
	/// @brief ワイヤーフレームのキューブ
	class DebugCube : public SingleMesh
	{
		/// @brief  キューブを作成
		void Make();
	public:
		DebugCube();
		~DebugCube() {}
	};
}
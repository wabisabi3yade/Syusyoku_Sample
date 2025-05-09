#pragma once
#include "SingleMesh.h"

namespace HashiTaku
{
	// キューブメッシュクラス
	class Cube : public SingleMesh
	{
		/// @brief キューブを作成
		void Make();
	public:

		Cube();
		~Cube() {};
	};
}
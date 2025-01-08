#pragma once
#include "VertexShader.h"

namespace HashiTaku
{
	/// @brief 地面描画の頂点シェーダ
	class VS_Ground : public VertexShader
	{

	public:
		VS_Ground();
		~VS_Ground() {}

		void OnceUpdateBuffer() override;
	};

}


#pragma once
#include "VertexShader.h"

namespace HashiTaku
{
	// グローシェーディングの頂点シェーダークラス
	class VS_Gouraud : public VertexShader
	{
	public:
		VS_Gouraud();
		~VS_Gouraud() {}

		void OnceUpdateBuffer() override;
	};
}
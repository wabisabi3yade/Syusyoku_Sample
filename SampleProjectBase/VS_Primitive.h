#pragma once
#include "VertexShader.h"

namespace HashiTaku
{
	/// @brief プリミティブで使用する頂点シェーダー
	class VS_Primitive : public VertexShader
	{
	public:
		VS_Primitive();
		~VS_Primitive() {}
	};
}
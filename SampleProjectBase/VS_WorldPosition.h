#pragma once
#include "VertexShader.h"

namespace HashiTaku
{
	class VS_WorldPosition : public VertexShader
	{

	public:
		VS_WorldPosition();
		~VS_WorldPosition() {}

		/// @brief 1ループに一回しか更新しなくてもよいバッファはここで更新する
		void OnceUpdateBuffer() override;
	};
}
#pragma once

#include "VertexShader.h"

namespace HashiTaku
{
	class VS_SkinnedAnimation : public VertexShader
	{
	public:
		VS_SkinnedAnimation();
		~VS_SkinnedAnimation() {}

		/// @brief 1ループに一回しか更新しなくてもよいバッファはここで更新する 
		void OnceUpdateBuffer() override;
	};
}
#pragma once

#include "VertexShader.h"

namespace HashiTaku
{
	class VS_SkinnedAnimation : public VertexShader
	{
	public:
		VS_SkinnedAnimation();
		~VS_SkinnedAnimation() {}

		/// @brief 1���[�v�Ɉ�񂵂��X�V���Ȃ��Ă��悢�o�b�t�@�͂����ōX�V���� 
		void OnceUpdateBuffer() override;
	};
}
#pragma once
#include "VertexShader.h"

namespace HashiTaku
{
	class VS_WorldPosition : public VertexShader
	{

	public:
		VS_WorldPosition();
		~VS_WorldPosition() {}

		/// @brief 1���[�v�Ɉ�񂵂��X�V���Ȃ��Ă��悢�o�b�t�@�͂����ōX�V����
		void OnceUpdateBuffer() override;
	};
}
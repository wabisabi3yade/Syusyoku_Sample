#pragma once
#include "PixelShader.h"

namespace HashiTaku
{
	/// @brief �w�i�̃I�u�W�F�N�g��PS
	class PS_BackObject : public PixelShader
	{
	public:
		PS_BackObject();
		~PS_BackObject() {}

		/// @brief �A�Z�b�g������
		void InitAssets() override;

		/// @brief ���݂̂̍X�V
		void OnceUpdateBuffer() override;
	};

}


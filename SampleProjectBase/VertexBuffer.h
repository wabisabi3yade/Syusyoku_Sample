#pragma once
#include "Buffer_Base.h"

namespace HashiTaku
{
	 /// @brief ���_�o�b�t�@�N���X
	class VertexBuffer : public Buffer_Base
	{
	public:
		VertexBuffer() {}
		~VertexBuffer() {}

		/// @brief ���_�o�b�t�@���쐬
		/// @param _allSize �S�̂̃T�C�Y
		/// @param _elementSize �v�f�̃T�C�Y
		/// @param pInitData ���_�f�[�^
		/// @return �����t���O
		bool CreateBuffer(u_int _allSize, u_int _elementSize, void* pInitData) override;

		/// @brief GPU�Ƀo�b�t�@�𑗂�
		void SetGPU() const override;
	};
}
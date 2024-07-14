#pragma once
#include "Buffer_Base.h"

// �C���f�b�N�X�o�b�t�@
class IndexBuffer : public Buffer_Base
{
public:
	IndexBuffer() {}
	~IndexBuffer() {}

	/// @brief �C���f�b�N�X�o�b�t�@���쐬
	/// @param _allSize �S�̂̃T�C�Y
	/// @param _elementSize �v�f�̃T�C�Y
	/// @param pInitData �C���f�b�N�X�f�[�^
	/// @return �����t���O
	bool CreateBuffer(u_int _allSize, u_int _elementSize, void* pInitData)override;

	void SetGPU() const override;
};


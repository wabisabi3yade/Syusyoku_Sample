#pragma once

// �o�b�t�@�̃C���^�[�t�F�[�X
class IBuffer
{
public:

	/// @brief �o�b�t�@���쐬
	/// @param size �T�C�Y
	/// @param pInitData  �f�[�^
	virtual bool CreateBuffer(UINT size, void* pInitData) = 0;

	/// @brief �o�b�t�@���擾����
	/// @return �o�b�t�@
	virtual const ID3D11Buffer& GetBuffer()const = 0;
};


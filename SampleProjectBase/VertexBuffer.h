#pragma once
#include "IBuffer.h"

//���_�o�b�t�@�N���X
class VertexBuffer : public IBuffer
{
	/// @brief ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;

public:
	VertexBuffer() {}
	~VertexBuffer() {}
	
	/// @brief ���_�o�b�t�@���쐬
	/// @param size �T�C�Y
	/// @param pInitData ���_�f�[�^
	/// @return �����t���O
	bool CreateBuffer(UINT size, void* pInitData) override;

	/// @brief ���_�o�b�t�@��Ԃ�
	/// @return ���_�o�b�t�@
	const ID3D11Buffer& GetBuffer()const override;
};


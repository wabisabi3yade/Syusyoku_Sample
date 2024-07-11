#pragma once

//ID3D11�o�b�t�@�̊��N���X
class Buffer_Base
{
protected:
	/// @brief �o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;

public:
	Buffer_Base() {};
	virtual ~Buffer_Base() {};
	
	/// @brief �o�b�t�@���쐬
	/// @param _allSize �S�̂̃T�C�Y
	/// @param _elementSize �v�f�̃T�C�Y
	/// @param pInitData �f�[�^
	/// @return �����t���O
	virtual bool CreateBuffer(u_int _allSize, u_int _elementSize, void* pInitData) = 0;

	/// @brief �o�b�t�@���擾����
	/// @return �o�b�t�@
	const ID3D11Buffer& GetBuffer()const;
};


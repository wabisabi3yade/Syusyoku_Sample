#pragma once

/// @brief �N���X�����擾����C���^�[�t�F�[�X
class IClassNameGetter
{
public:
	/// @brief �N���X����Ԃ�
	/// @return �N���X��
	virtual std::string ClassNameToStr() = 0;
};


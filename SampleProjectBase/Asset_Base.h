#pragma once

// �A�Z�b�g�̊��N���X
class Asset_Base
{
protected:

	/// @brief �A�Z�b�g�̖��O
	std::string name;
public:

	/// @brief �R���X�g���N�^
	/// @param _type �A�Z�b�g�̎��
	Asset_Base() : name("") {};
	virtual ~Asset_Base() {};

	/// @brief ���O���Z�b�g����
	/// @param _name �A�Z�b�g�̖��O
	void SetAssetName(const std::string& _name);

	/// @brief ���O���擾����
	/// @return �A�Z�b�g�̖��O
	std::string GetAssetName()const;
};
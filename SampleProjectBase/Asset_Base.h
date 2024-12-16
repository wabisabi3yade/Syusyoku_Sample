#pragma once
#include "ISaveLoad.h"

namespace HashiTaku
{
	// �A�Z�b�g�̊��N���X
	class Asset_Base : public ISaveLoad
	{
		/// @brief �A�Z�b�g�̖��O
		std::string assetName;

		/// @brief �Z�[�u���邩�H
		bool isSave;
	public:

		/// @brief �R���X�g���N�^
		/// @param _type �A�Z�b�g�̎��
		Asset_Base() : assetName(""), isSave(true) {};
		virtual ~Asset_Base() {};

		/// @brief ���O���Z�b�g����
		/// @param _name �A�Z�b�g�̖��O
		void SetAssetName(const std::string& _name);

		/// @brief ���O���擾����
		/// @return �A�Z�b�g�̖��O
		const std::string& GetAssetName() const;

		// �Z�[�u���邩�擾
		bool GetIsSave() const;

		/// @brief �Z�[�u����
		/// @return �Z�[�u�f�[�^
		nlohmann::json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����f�[�^ 
		void Load(const nlohmann::json& _data) override;

		/// @brief �Z�[�u���Ȃ��悤�ɂ���
		void SetIsntSave();
	};
}
#pragma once
#include "Asset_Base.h"

namespace HashiTaku
{
	/// @brief �p�X������쐬����A�Z�b�g���
	class AssetPath_Base : public Asset_Base
	{
		/// @brief �p�X��
		std::string pathName;
	public:
		AssetPath_Base() : pathName("") {}
		virtual ~AssetPath_Base() {}

		// �p�X�����Z�b�g
		void SetPathName(const std::string& _pathName);

		// �p�X�����擾
		std::string GetPathName() const;

		/// @brief �Z�[�u����
		/// @param _sceneData �Z�[�u�f�[�^
		nlohmann::json Save() override;

		/// @brief ���[�h����
		/// @param _sceneData ���[�h����f�[�^ 
		void Load(const nlohmann::json& _data) override;
	};
}
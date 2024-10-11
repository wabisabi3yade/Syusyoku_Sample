#pragma once
#include "SaveJson.h"

namespace HashiTaku
{
	/// @brief �Z�[�u�E���[�h������C���^�[�t�F�[�X
	class ISaveLoad
	{
	public:
		ISaveLoad() {}
		virtual ~ISaveLoad() {}

		/// @brief �Z�[�u����
		/// @return �Z�[�u�f�[�^
		virtual nlohmann::json Save() = 0;

		/// @brief ���[�h����
		/// @param _data ���[�h����f�[�^ 
		virtual void Load(const nlohmann::json& _data) = 0;

		/*nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;*/
	};
}



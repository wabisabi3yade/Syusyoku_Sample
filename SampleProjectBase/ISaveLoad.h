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
		virtual json Save() = 0;

		/// @brief ���[�h����
		/// @param _data ���[�h����f�[�^ 
		virtual void Load(const json& _data) = 0;

		/*json Save() override;
		void Load(const json& _data) override;*/
	};
}



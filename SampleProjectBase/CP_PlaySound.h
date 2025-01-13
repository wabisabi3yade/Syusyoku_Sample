#pragma once
#include "Component.h"

namespace HashiTaku
{
	/// @brief �J�n���ɉ����Đ�����
	class CP_PlaySound : public Component
	{
		/// @brief �Đ����鉹�̃p�����[�^
		PlaySoundParameter soundParameter;

		/// @brief �Đ���������ID
		int soundId;

		/// @brief BGM�Ƃ��čĐ����邩�H
		bool isBGM;
	public:
		CP_PlaySound();
		~CP_PlaySound() {}

		/// @brief �폜������
		void OnDestroy() override;

		/// @brief �Z�[�u����
		/// @param _data �Z�[�u�V�[���f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����V�[���f�[�^ 
		void Load(const json& _data) override;
	private:
		/// @brief �J�n����
		void Start() override;

		void ImGuiDebug() override;
	};
}


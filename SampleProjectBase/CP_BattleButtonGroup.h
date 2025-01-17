#pragma once
#include "CP_ButtonGroup.h"

namespace HashiTaku
{
	class CP_Fade;

	// �o�g�����̃{�^��(�|�[�Y�A�Q�[���I�[�o�[����)
	class CP_BattleButtonGroup : public CP_ButtonGroup
	{
		enum class ButtonType
		{
			Retry,
			Title,
			Back,
			MaxNum
		};

	public:
		CP_BattleButtonGroup();
		~CP_BattleButtonGroup() {}

		/// @brief �t�F�[�h�I�u�W�F�N�g��
		std::string fadeObjName;
	
		/// @brief �w�i�I�u�W�F�N�g��
		std::string frontFadeName;

		/// @brief �t�F�[�h�I�u�W�F�N�g
		CP_Fade* pFade; 

		/// @brief ��O�̃t�F�[�h�I�u�W�F�N�g
		CP_Fade* pFrontFade;

		/// @brief �{�^������������̃t�F�[�h����鎞��
		float closeFadeTime;

		/// @brief �|�[�Y���̃t�F�[�h�Ƀ��l
		float poseFadeAlpha;

		/// @brief �{�^����\������
		void OpenDisplay() override;

		/// @brief �{�^�������
		void CloseDisplay() override;

		json Save() override;
		void Load(const json& _data) override;

	private:
		void Start() override;

		/// @brief �I�u�W�F�N�g��T��
		void FindObject();

		/// @brief �{�^���̏�����
		void ButtonSetup();

		/// @brief ���g���C�{�^�����������Ƃ��̏���
		void OnPushRetry();

		/// @brief ���j���[�{�^�����������Ƃ��̏���
		void OnPushTitle();

		void ImGuiDebug() override;
	};
}



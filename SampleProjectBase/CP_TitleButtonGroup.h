#pragma once
#include "CP_ButtonGroup.h"

namespace HashiTaku
{
	class CP_Fade;

	/// @brief �^�C�g���̃{�^���Ǘ�
	class CP_TitleButtonGroup : public CP_ButtonGroup
	{
		/// @brief �^�C�g���Ŏg�p����{�^���̎��
		enum class ButtonType
		{
			Start,
			End,
			Check,
			MaxNum
		};

		/// @brief �t�F�[�h�I�u�W�F�N�g��
		std::string fadeObjName;

		/// @brief �`�F�b�N�}�[�N�I�u�W�F�N�g��
		std::string checkImageObjName;

		/// @brief �t�F�[�h
		CP_Fade* pFade;

		/// @brief �`�F�b�N�}�[�N�̃C���[�W
		CP_UIRenderer* pCheckImage;
	public:
		CP_TitleButtonGroup();
		~CP_TitleButtonGroup() {}

		json Save() override;
		void Load(const json& _data) override;
	private:
		void Start() override;

		/// @brief �I�u�W�F�N�g��T��
		void FindObject();

		/// @brief �X�^�[�g�{�^�����������Ƃ��̏���
		void OnPushStart();

		/// @brief �I���{�^�����������Ƃ��̏���
		void OnPushEnd();

		/// @brief �`�F�b�N�}�[�N���������Ƃ��̏���
		void OnPushCheck();

		/// @brief �{�^���̏�����
		void ButtonSetup();

		void ImGuiDebug() override;
	};
}
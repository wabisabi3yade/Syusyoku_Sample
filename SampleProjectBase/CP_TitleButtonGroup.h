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
			MaxNum
		};

		/// @brief �t�F�[�h�I�u�W�F�N�g��
		std::string fadeObjName;

		/// @brief �t�F�[�h
		CP_Fade* pFade;
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

		/// @brief �{�^���̏�����
		void ButtonSetup();

		/// @brief �o�g���V�[���Ɉڍs
		void ChangeBattleScene();

		void ImGuiDebug() override;
	};
}
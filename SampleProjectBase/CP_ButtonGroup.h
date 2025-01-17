#pragma once
#include "CP_Button.h"
#include "CP_UIRenderer.h"

namespace HashiTaku
{
	/// @brief �{�^�����O���[�v�����āA�Ǘ�����
	class CP_ButtonGroup : public Component
	{
		/// @brief �{�^���̃p�����[�^
		struct ButtonParameter
		{
			/// @brief �{�^���R���|�[�l���g
			CP_Button* pButton{ nullptr };

			/// @brief �\������Ƃ��̃{�^���̍��W
			DXSimp::Vector3 buttonPos;
		};

		/// @brief �Z���N�g�C���[�W�̍��W��I�t�Z�b�g�i�{�^���̍��W����j
		std::vector<DXSimp::Vector3> selectOffsetPosFromButton;

		/// @brief �f�t�H���g�̃{�^����ID
		u_int defaultSelectButtonId;
	protected:
		/// @brief �{�^���p�����[�^�̃O���[�v
		std::vector<ButtonParameter> buttonParameterGroup;

		/// @brief �{�^���̃R���|�[�l���g�����I�u�W�F�N�g
		std::vector<std::string> buttonObjNames;

		/// @brief �Z���N�g���Ă鎞�̉摜
		CP_UIRenderer* pSelectBackImage;

		/// @brief �Z���N�g���Ă鎞�̉摜�I�u�W�F�N�g��
		std::string backImageObjName;

		/// @brief ���ݑI��ł���{�^����ID
		int curSelectButtonId;

		/// @brief �{�^���̐�
		int maxButtonCnt;

		/// @brief �c���͂ňړ�����{�^���̐�
		int vertMoveSpeed;

		/// @brief �����͂ňړ�����{�^���̐�
		int horiMoveSpeed;

		/// @brief �ړ����邱�Ƃ��ł��邩
		bool canInput;
	public:
		CP_ButtonGroup();
		virtual ~CP_ButtonGroup() {}

		/// @brief �ړ��ł��邩�Z�b�g����
		/// @param _canMove �ړ��ł��邩�t���O
		void SetCanMove(bool _canMove);

		/// @brief �{�^����\������
		virtual void OpenDisplay();

		/// @brief �{�^�������
		virtual void CloseDisplay();

		json Save() override;
		void Load(const json& _data) override;
	protected:
		void Start() override;

		void Update() override;

		/// @brief ���͂őI���{�^�����ړ�����
		void MoveButton();

		/// @brief �{�^��������
		void DecideButton();

		/// @brief �Z���N�g�̔w�i�C���[�W��I�𒆂̃{�^���ֈړ�
		void MoveSelectBackImage();

		/// @brief �ݒ肵�Ă���{�^���̍��W�փ{�^���摜���ړ�������
		void SettingButtonPosition();

		/// @brief �{�^����\�������邩�H
		/// @param _isDisplay �\�������邩�H
		virtual void SetDisplayButtons(bool _isDisplay);

		/// @brief �ő吔���Z�b�g
		/// @param _max �ő吔
		void SetMaxButtonCnt(int _max);

		void ImGuiDebug() override;
	};
}
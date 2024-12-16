#pragma once
#include "Scene.h"

namespace HashiTaku
{
	/// @brief �ҏW������V�[���N���X
	class EditScene : public Scene, IImGuiUser
	{
		/// @brief �V�[���Đ������H
		bool isPlaying;

		/// @brief  �|�[�Y�����H
		bool isPause;

		/// @brief 1�t���[�������Đ������H
		bool isOneFrameProgressing;
	public:
		/// @brief �R���X�g���N�^
		/// @param _sceneName �V�[����
		/// @param _frameRateClass �σt���[�����[�g�N���X
		EditScene(const std::string& _name);
		~EditScene() {}

		/// @brief ���s�֐�
		void Exec() override;

	private:
		bool CanSceneUpdate();

		/// @brief �Z�[�u�@�\
		void SceneSave();

		// �Z�[�u�t�@�C���̃p�X��
		std::string SaveFilePath();

		/// @brief ImGui�ɂ��V�[�����̃p�����[�^�ҏW�Ȃǂ̊J���֐�
		void ImGuiDebug() override;

		/// @brief �Đ����̕\��
		void ImGuiPlaying();

		/// @brief ��~���̕\��
		void ImGuiStop();

		/// @brief �Đ����n�߂�
		void PlayStart();

		/// @brief �Đ����~�߂�
		void PlayEnd();

		/// @brief �Đ����|�[�Y������A�J�n������
		void PlayPauseAndBegin();

		/// @brief �J�n����
		void StartProgressOneFrame();
	};
}
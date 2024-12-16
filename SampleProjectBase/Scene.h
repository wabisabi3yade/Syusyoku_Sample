#pragma once
#include "BulletTest.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	// 1�V�[���N���X
	class Scene
	{
	protected:
		/// @brief �V�[����
		std::string sceneName;

		/// @brief �V�[�����V�X�e���}�l�W���[
		InSceneSystemManager* pInSceneSystem;

		/// @brief �G�t�F�N�g�}�l�W���[
		DX11EffecseerManager* pEffectManager;
	public:
		Scene(const std::string& _name);
		virtual ~Scene();

		/// @brief ���s�֐�
		virtual void Exec();

		// �V�[�����ݒ�
		void SetName(const std::string& _name);

		// ���O���擾
		std::string GetName() const;

	protected:
		/// @brief �V�[���Ȃ��X�V
		void SceneUpdate();

		/// @brief �V�[�����`��
		void SceneDraw();

		/// @brief json�t�@�C������V�[�������[�h
		void SceneLoad();
	private:
		/// @brief �`�揀��
		void DrawSetup();

		// �Z�[�u�t�@�C���̃p�X��
		std::string SaveFilePath();
	};
}
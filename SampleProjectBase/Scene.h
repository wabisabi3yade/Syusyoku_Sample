#pragma once
#include "BulletTest.h"

class InSceneSystemManager;
class EditScene;

// 1�V�[���N���X
class Scene /*: public HashiTaku::IImGuiUser*/
{
protected:
	/// @brief �V�[����
	std::string sceneName;

	/// @brief �V�[�����V�X�e���}�l�W���[
	InSceneSystemManager* pInSceneSystem;
public:
	Scene(const std::string& _name);
	virtual ~Scene();

	/// @brief ���s�֐�
	virtual void Exec();

	///// @brief �Z�[�u�@�\
	//void Save();

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

	/// @brief �V�[���Đ����邩�H
	/// @return �Đ����邩�H
	bool IsUpdatePlay();

	///// @brief �Đ����̕\��
	//void ImGuiPlaying();

	///// @brief ��~���̕\��
	//void ImGuiStop();

	///// @brief �Đ����n�߂�
	//void PlayStart();

	///// @brief �Đ����~�߂�
	//void PlayEnd();
};


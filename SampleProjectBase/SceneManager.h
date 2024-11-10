#pragma once
#include "Singleton_Base.h"

#include "EditScene.h"

class SceneManager : public Singleton_Base<SceneManager>, public HashiTaku::IImGuiUser
{
	friend class Singleton_Base<SceneManager>;

	/// @brief �V�[�����X�g
	std::list<std::string> sceneList;

	/// @brief ���݂̃V�[����
	std::string nowSceneName;

	/// @brief ���ݍĐ����Ă���V�[��
	std::unique_ptr<Scene> pNowScene;

	SceneManager();
	~SceneManager();
	
public:
	// ���s�֐�
	void Exec();

	/// @brief �V�[����ύX����
	/// @param _sceneName �V�[����
	/// @param _isEditScene EditScene���H
	void ChangeScene(const std::string& _sceneName, bool _isEditScene = false);

private:
	/// @brief  ����
	void Setup();

	/// @brief �S�V�[����������
	void SetupSceneList();

	/// @brief �A�Z�b�g�̏���
	void AssetSetup();

	/// @brief �}�e���A���̏���
	void MaterialSetup();

	// �������
	void Release();

	/// @brief �V�[�����쐬
	/// @param _sceneName �V�[����
	void CreateScene(const std::string& _sceneName);

	void ImGuiDebug() override;
	void ImGuiChangeScene();
	void ImGuiCreateScene();
};


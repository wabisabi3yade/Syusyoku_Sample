#include "pch.h"
#include "Scene.h"

#include "InSceneSystemManager.h"
#include "ShaderCollection.h"



Scene::Scene(const std::string& _name)
{
	pInSceneSystem = InSceneSystemManager::GetInstance();
	pInSceneSystem->Init();

	sceneName = _name;

	// ���[�h����
	Load();
}

Scene::~Scene()
{
	InSceneSystemManager::Delete();
}

void Scene::Exec()
{
	SceneObjects& sceneObjects = pInSceneSystem->GetSceneObjects();

	// �V�[�����̍X�V����
	sceneObjects.Update();

	// Update�̌�ɍs���X�V����(�J�����ɏ����Ƃ�)
	sceneObjects.LateUpdate();

	// ImGui�ҏW
	sceneObjects.ImGuiSetting();

	// �`��O����
	DrawSetup();

	// �V�[�����̕`�揈��
	sceneObjects.Draw();
}

void Scene::Save()
{
	// �V�[���f�[�^
	nlohmann::json sceneData;

	// �V�[�����I�u�W�F�N�g���Z�[�u
	SceneObjects& sceneObj = pInSceneSystem->GetSceneObjects();
	sceneData["objects"] = sceneObj.SaveObject();

	std::ofstream f(SaveFilePath());

	auto str = sceneData.dump(4);
	auto len = str.length();
	f.write(str.c_str(), len);
}

void Scene::Load()
{
	// �t�@�C������json��ǂݍ���
	std::ifstream f(SaveFilePath());

	if (!f.is_open())
	{
		HASHI_DEBUG_LOG("�V�[���Z�[�u�t�@�C�����J���܂���ł���");
		return;
	}

	const nlohmann::json sceneData = nlohmann::json::parse(f);

	// �V�[���I�u�W�F�N�g�����[�h
	const nlohmann::json& sceneObjData = sceneData["objects"];
	InSceneSystemManager::GetInstance()->GetSceneObjects().LoadObject(sceneObjData);

	HASHI_DEBUG_LOG(sceneName + " ���[�h����");
}

void Scene::SetName(const std::string& _name)
{
	sceneName = _name;
}

std::string Scene::GetName() const
{
	return sceneName;
}

void Scene::DrawSetup()
{
	SceneLights& sceneLights = pInSceneSystem->GetSceneLights();

	// ��ʃN���A�ȂǏ���
	Direct3D11::GetInstance()->GetRenderer()->SetUpDraw();

	// �����̍X�V����
	sceneLights.Update();

	// 1���[�v1�x�����X�V����΂����o�b�t�@�X�V
	ShaderCollection* shCol = ShaderCollection::GetInstance();
	shCol->UniqueUpdateBuffer();
}

std::string Scene::SaveFilePath()
{
	// �t�@�C������ݒ�
	std::string fileName = "assets/data/scene/";	// �p�X
	fileName += sceneName;	// �t�@�C����
	fileName += ".json";	// �g���q

	return fileName;
}

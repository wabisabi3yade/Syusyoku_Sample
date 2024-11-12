#include "pch.h"
#include "Scene.h"

#include "InSceneSystemManager.h"
#include "ShaderCollection.h"
#include "DX11BulletPhisics.h"
#include "Geometory.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Scene::Scene(const std::string& _sceneName) : sceneName(_sceneName)
{
	pInSceneSystem = InSceneSystemManager::GetInstance();
	pInSceneSystem->Init();

	// ���[�h����
	SceneLoad();
}

Scene::~Scene()
{
	InSceneSystemManager::Delete();
}

void Scene::Exec()
{
	// �V�[�����X�V
	SceneUpdate();

	// �V�[���`��
	SceneDraw();
}

void Scene::SceneUpdate()
{
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();
	DX11BulletPhisics* pBulletEngine = DX11BulletPhisics::GetInstance();

	// �Q�[�������͍X�V����
	pInSceneSystem->InputUpdate();

	// �J�n����
	sceneObjects.Awake();
	sceneObjects.Start();

	// �����V�~�����[�V������i�߂�
	pBulletEngine->Update();

	// �V�[�����̍X�V����
	sceneObjects.Update();
	sceneObjects.LateUpdate();

	// Dx�̃g�����X�t�H�[���� Bullet�ɍ��킹��
	pBulletEngine->UpdateTransformDxToBt();
}

void Scene::SceneDraw()
{
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();

	// �`��O����
	DrawSetup();

	// ���`��
	Geometory::DrawLine();

	// �V�[�����̕`�揈��
	sceneObjects.Draw();
}

void Scene::SceneLoad()
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
	pInSceneSystem->GetSceneObjects().LoadObjectList(sceneObjData);

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
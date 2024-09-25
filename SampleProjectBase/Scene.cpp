#include "pch.h"
#include "Scene.h"

#include "InSceneSystemManager.h"
#include "ShaderCollection.h"
#include "DX11BulletPhisics.h"
#include "AssetSaveLoad.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Scene::Scene(const std::string& _name) : isPlay(true)
{
	pInSceneSystem = InSceneSystemManager::GetInstance();
	pInSceneSystem->Init();

	sceneName = _name;

	// ImGui�g�p����Ȃ�
	if (HashiTaku::IImGuiUser::GetIsImGuiUse())
		isPlay = false;

	// ���[�h����
	Load();

	/*test = new BulletTest();*/
}

Scene::~Scene()
{
	InSceneSystemManager::Delete();
	/*delete test;*/
}

void Scene::Exec()
{
	SceneObjects& sceneObjects = pInSceneSystem->GetSceneObjects();

	// �Đ����݂̂̍X�V
	PlayOnlyUpdate();

	// ImGui�ҏW
	sceneObjects.ImGuiSetting();

	// �`��O����
	DrawSetup();

	// �V�[�����̕`�揈��
	sceneObjects.Draw();
}

void Scene::PlayOnlyUpdate()
{
	if (!IsUpdatePlay()) return;	// �Đ����Ȃ�

	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();

	// Bullet�̃g�����X�t�H�[����Dx�ɍ��킹��
	pInSceneSystem->UpdateTransformBtToDx();

	// �J�n����
	sceneObjects.Awake();
	sceneObjects.Start();

	// �����V�~�����[�V������i�߂�
	DX11BulletPhisics::GetInstance()->Update();

	/*test->Update();*/

	// �V�[�����̍X�V����
	sceneObjects.Update();
	sceneObjects.LateUpdate();

	// �Փ˃I�u�W�F�N�g�̃R�[���o�b�N
	/*DX11BulletPhisics::GetInstance()->CollisionCallBack();*/
}

void Scene::ImGuiSetting()
{
	if (isPlay)
		ImGuiPlaying();
	else
		ImGuiStop();
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

	HASHI_DEBUG_LOG(sceneName + " �Z�[�u���܂���");

	AssetSaveLoader::Save();	// �A�Z�b�g�Z�[�u
}

bool Scene::GetIsUpdatePlay() const
{
	return isPlay;
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
	pInSceneSystem->GetSceneObjects().LoadObject(sceneObjData);

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

bool Scene::IsUpdatePlay()
{
#ifdef EDIT
	if (!isPlay) return false;
#endif // EDIT

	return true;
}

void Scene::ImGuiPlaying()
{
#ifdef EDIT
	if (ImGui::Button("Stop"))
		PlayStop();
#endif // EDIT
}

void Scene::ImGuiStop()
{
#ifdef EDIT
	if (ImGui::Button("Play"))
		PlayStart();

	// �ȉ��Đ����͕\�����Ȃ�
	if (ImGui::Button("Save"))
	{
		Save();
	}
#endif // EDIT
}

void Scene::PlayStart()
{
	isPlay = true;

	// �V�[���Đ��O�ɃZ�[�u����
	Save();
}

void Scene::PlayStop()
{
	isPlay = false;

	// �V�����V�[���̒��𐶐�����
	InSceneSystemManager::GetInstance()->Reset();
	Load();
}

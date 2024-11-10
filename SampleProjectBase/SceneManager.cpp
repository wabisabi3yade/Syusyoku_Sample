#include "SceneManager.h"

#include "GameInput.h"
#include "ComponentFactory.h"
#include "DX11BulletPhisics.h"
#include "AnimationNotifyFactory.h"

// �A�Z�b�g������
#include "AssetSetter.h"
#include "Material.h"
#include "Geometory.h"

namespace fs = std::filesystem;

SceneManager::SceneManager() : nowSceneName("")
{
	Setup();

	ChangeScene("test", true);
}

SceneManager::~SceneManager()
{
	Release();
}

void SceneManager::Setup()
{
	// Bullet�����G���W��������
	DX11BulletPhisics::GetInstance()->Init();

	// �A�Z�b�g�֘A
	AssetSetup();

	// �R���|�[�l���g������
	ComponentFactory::GetInstance()->Init();

	// �V�[�����X�g������
	SetupSceneList();
}

void SceneManager::SetupSceneList()
{
	std::string folderPath = "assets/data/scene";

	// �V�[���t�H���_�ɂ���V�[���t�@�C�����擾����
	for (const auto& entry : fs::recursive_directory_iterator(folderPath))
	{
		if (!entry.is_regular_file() || entry.path().extension() != ".json") continue;

		// �t�@�C���̃p�X���o��
		sceneList.push_back(entry.path().stem().string());
	}
}

void SceneManager::AssetSetup()
{
	// �f�t�H�ł���}�e���A�����쐬
	MaterialSetup();

	// ��{�I�u�W�F�N�g�̏�����
	Geometory::Init();
}

void SceneManager::Release()
{
	pNowScene.reset();
	ComponentFactory::Delete();
	Geometory::Release();
	DX11BulletPhisics::Delete();
}

void SceneManager::CreateScene(const std::string& _sceneName)
{
	auto itr = std::find(sceneList.begin(), sceneList.end(), _sceneName);

	if (itr != sceneList.end())	// �V�[�������d�����Ă�����
	{
		HASHI_DEBUG_LOG(_sceneName + "���ɃV�[�������g���Ă��܂�");
		return;
	}

	sceneList.push_back(_sceneName);
}

void SceneManager::ImGuiDebug()
{
#ifdef EDIT
	ImGui::Begin("SceneList");

	ImGuiChangeScene();

	ImGuiCreateScene();

	ImGui::End();
#endif
}

void SceneManager::ImGuiChangeScene()
{
#ifdef EDIT
	ImGui::Text(TO_UTF8("�V�[���� " + nowSceneName));
	
	// �Đ��݂̂̃V�[��
	ImGui::Text("Play Only");
	for (auto& name : sceneList)
	{
		if (ImGui::Button(name.c_str()))
			ChangeScene(name);
	}

	ImGui::Text("--------------");

	// �ҏW�ł���V�[��
	ImGui::Text("Edit");
	ImGui::PushID("Edit");
	for (auto& name : sceneList)
	{
		if (ImGui::Button(name.c_str()))
			ChangeScene(name, true);
	}
	ImGui::PopID();
#endif
}

void SceneManager::ImGuiCreateScene()
{
#ifdef EDIT
	constexpr u_int buf = 256;
	static char input[buf];
	ImGui::InputText("name", input, buf);

	if (ImGui::Button("New Scene"))
		CreateScene(input);
#endif
}

void SceneManager::Exec()
{
	// �V�[���̎��s
	pNowScene->Exec();

	ImGuiCall();
}

void SceneManager::ChangeScene(const std::string& _sceneName, bool _isEditScene)
{
	auto itr = std::find(sceneList.begin(), sceneList.end(), _sceneName);

	if (itr == sceneList.end())	// �V�[�������Ȃ��Ȃ�
	{
		HASHI_DEBUG_LOG(_sceneName + "�V�[����������܂���");
		return;
	}

	pNowScene.reset();
	nowSceneName = _sceneName;

	// �ҏW����V�[�����ǂ���
	if (!_isEditScene)
		pNowScene = std::make_unique<Scene>(_sceneName);
	else
		pNowScene = std::make_unique<EditScene>(_sceneName);

	HASHI_DEBUG_LOG(_sceneName + "�ֈڍs");
}

void SceneManager::MaterialSetup()
{
	// Unlit�}�e���A���쐬
	std::unique_ptr<Material> pUnlit = std::make_unique<Material>();
	pUnlit->SetIsntSave();
	AssetSetter::SetAsset("M_Unlit", std::move(pUnlit));
}

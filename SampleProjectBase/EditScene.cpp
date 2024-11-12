#include "pch.h"
#include "EditScene.h"
#include "AssetSaveLoad.h"
#include "InSceneSystemManager.h"

EditScene::EditScene(const std::string& _sceneName) :
	Scene(_sceneName),
	isPlaying(false),
	isPause(false),
	isOneFrameProgressing(false)

{
}

void EditScene::Exec()
{
	SceneObjects& sceneObjects = pInSceneSystem->GetSceneObjects();

	// �Đ����̂݃V�[���X�V
	if (CanSceneUpdate())
	{
		SceneUpdate();
		isOneFrameProgressing = false;
	}

	// ImGui�ɂ��ҏW
	ImGuiCall();

	// �V�[���`��
	SceneDraw();
}

bool EditScene::CanSceneUpdate()
{
	if ((isPlaying && !isPause) || isOneFrameProgressing)
		return true;

	return false;
}

void EditScene::SceneSave()
{
	// �V�[���f�[�^
	nlohmann::json sceneData;

	// �V�[�����I�u�W�F�N�g���Z�[�u
	SceneObjects& sceneObj = pInSceneSystem->GetSceneObjects();
	sceneData["objects"] = sceneObj.SaveObjectList();

	std::ofstream f(SaveFilePath());

	auto str = sceneData.dump(4);
	auto len = str.length();
	f.write(str.c_str(), len);

	HASHI_DEBUG_LOG(sceneName + " �Z�[�u���܂���");

	AssetSaveLoader::Save();	// �A�Z�b�g�Z�[�u
}

std::string EditScene::SaveFilePath()
{
	// �t�@�C������ݒ�
	std::string fileName = "assets/data/scene/";	// �p�X
	fileName += sceneName;	// �t�@�C����
	fileName += ".json";	// �g���q

	return fileName;
}

void EditScene::ImGuiDebug()
{
#ifdef EDIT
	SceneObjects& sceneObjects = pInSceneSystem->GetSceneObjects();
	SceneLights& sceneLights = pInSceneSystem->GetSceneLights();
	DX11BulletPhisics* pBulletPhisics = DX11BulletPhisics::GetInstance();

	// �I�u�W�F�N�g��ImGui�ҏW
	sceneObjects.ImGuiCall();

	// ���C�e�B���O��ImGUi�ҏW
	sceneLights.ImGuiCall();

	ImGui::Begin("Edit Play");
	// �Đ����ƂłȂ��Ƃ��ŏo���E�B���h�E��ς���
	if (isPlaying)
		ImGuiPlaying();
	else
		ImGuiStop();

	ImGui::End();

	// �����蔻��`��
	pBulletPhisics->Draw();

	// �V�X�e���\�� 
	AppSystemDraw::GetInstance()->ImGuiCall();

	// �f�o�b�O���b�Z�[�W�\��
	ImGuiDebugLog::DisplayMessage();
#endif // EDIT
}

void EditScene::ImGuiPlaying()
{
#ifdef EDIT
	InputClass& input = MainApplication::GetInput();

	// �I���
	if (ImGui::Button("End"))
		PlayEnd();

	// �|�[�Y
	if (ImGui::Button("Pause") || input.GetKeyboard().GetKeyDown(DIK_P))
		isPause = !isPause;

	// 1�t���[���Đ�
	if (ImGui::Button("Progress") || input.GetKeyboard().GetKeyDown(DIK_N))
		StartProgressOneFrame();
#endif // EDIT
}

void EditScene::ImGuiStop()
{
#ifdef EDIT
	if (ImGui::Button("Play"))
		PlayStart();

	// �ȉ��Đ����͕\�����Ȃ�
	if (ImGui::Button("Save"))
	{
		SceneSave();
	}
#endif // EDIT
}

void EditScene::PlayStart()
{
	isPlaying = true;

	// �V�[���Đ��O�ɃZ�[�u����
	SceneSave();
}

void EditScene::PlayEnd()
{
	isPlaying = false;
	isPause = false;
	isOneFrameProgressing = false;

	// �V�����V�[���̒��𐶐�����
	InSceneSystemManager::GetInstance()->Reset();
	SceneLoad();
}

void EditScene::PlayPauseAndBegin()
{
	isPlaying = !isPlaying;
}

void EditScene::StartProgressOneFrame()
{
	isOneFrameProgressing = true;
}


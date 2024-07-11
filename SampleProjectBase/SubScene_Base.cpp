#include "SubScene_Base.h"
#include "SceneMoveInfo.h"

// �J����
#include "Camera.h"

// �V�X�e���n
#include "GameMode.h"
#include "InSceneSystemManager.h"


#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"
#include "ImGuiMethod.h"

void SubScene_Base::DrawSetup()
{
	SceneObjects& sceneObjects = systemManager->GetSceneObjects();
	SceneLights& sceneLights = systemManager->GetSceneLights();

	// ��ʃN���A�ȂǏ���
	Direct3D11::GetInstance()->GetRenderer()->SetUpDraw();

	// �r���[�ϊ��s����X�V
	Camera* mainCamera = sceneObjects.GetSceneObject<Camera>("MainCamera");
	mainCamera->UpdateViewMatrix();

	// �����̍X�V����
	sceneLights.Update();
}

/// <summary>
/// �V�[���ړ�����ƂȂ�����Ăяo��
/// </summary>
/// <param name="_nextSubType">���̃T�u�V�[���̃^�C�v</param>
/// <param name="_nextBroadType">���̑�ǃV�[���̃^�C�v</param>
void SubScene_Base::OnMoveScene(int _nextSubType, BroadType::Type _nextBroadType)
{
	// �V�[���J�ڏ���J�ڐ�ɕς���
	pSceneMoveInfo->OnChangeScene(_nextSubType, _nextBroadType);
}

SubScene_Base::SubScene_Base(SceneMoveInfo* _pSceneMoveInfo)
{
	pSceneMoveInfo = _pSceneMoveInfo;

	// �V�X�e��������
	systemManager = InSceneSystemManager::GetInstance();
	systemManager->Init();

	// �C���X�^���X����
	std::unique_ptr<Camera> mainCamera = std::make_unique<Camera>();


	// �e�C���X�^���X��n��
	InSceneSystemManager* system = InSceneSystemManager::GetInstance();

	// �J�����ݒ�
	Camera* camPtr = mainCamera.get();
	GameMode::GetInstance()->SetCamera(*camPtr);	// �Q�[�����[�h�ɃJ������ݒ肷��

	camPtr->SetName("MainCamera");

	SceneObjects& sceneObjects = systemManager->GetSceneObjects();
	sceneObjects.SetObject(std::move(mainCamera));
}

SubScene_Base::~SubScene_Base()
{
	// �V�[���̃V�X�e����j��
	systemManager->Delete();

	// �V�[�����ƂɃQ�[�����[�h����蒼��
	GameMode::Delete();
}


void SubScene_Base::Exec()
{
	SceneObjects& sceneObjects = systemManager->GetSceneObjects();
	CollisionChecker& collisionChecker = systemManager->GetCollisonChecker();

	// �V�[�����̍X�V����
	Update();
	sceneObjects.Update();

	// Update�̌�ɍs���X�V����(�J�����ɏ����Ƃ�)
	LateUpdate();
	sceneObjects.LateUpdate();

	// �V�[�����̓����蔻����`�F�b�N����
	collisionChecker.CollisionCheck();

	// �`��O����
	DrawSetup();

	// �V�[�����̕`�揈��
	Draw();
	sceneObjects.Draw();
	
}

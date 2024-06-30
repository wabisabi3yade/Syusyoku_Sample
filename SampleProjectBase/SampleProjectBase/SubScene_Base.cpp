#include "SubScene_Base.h"
#include "SceneMoveInfo.h"

#include "Camera.h"
#include "CollisionRegister.h"
#include "GameMode.h"
#include "SObjectRegister.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"
#include "ImGuiMethod.h"

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
	resourceCollection = ResourceCollection::GetInstance();

	// �C���X�^���X����
	sceneObjects = std::make_unique<SceneObjects>();
	sceneLights = std::make_unique<SceneLights>();
	collisionChcker = std::make_unique<CollisionChecker>();
	std::unique_ptr<Camera> mainCamera = std::make_unique<Camera>();

	// �V�[���I�u�W�F�N�g�o�^�N���X�Z�b�g
	SObjectRegister::GetInstance()->SetSceneObjects(*sceneObjects.get());

	// �����蔻��`�F�b�J�[�N���X�𐶐����A�����蔻��o�^�Ώۂɂ���
	CollisionRegister::GetInstance()->SetCollisionChecker(*collisionChcker);

	// �J�����ݒ�
	Camera* camPtr = mainCamera.get();
	GameMode::GetInstance()->SetCamera(*camPtr);	// �Q�[�����[�h�ɃJ������ݒ肷��
	sceneObjects->SetObject("MainCamera", std::move(mainCamera));
}

SubScene_Base::~SubScene_Base()
{
	// �V�[�����̃I�u�W�F�N�g�̏I���������s��
	sceneObjects.reset();	

	// �V�[�����ƂɃQ�[�����[�h����蒼��
	GameMode::Delete();

	// �o�^�N���X�������Ă���
	CollisionRegister::Delete();
	SObjectRegister::Delete();
}


void SubScene_Base::Exec()
{
	// �V�[�����̍X�V����
	Update();
	sceneObjects->Update();

	// Update�̌�ɍs���X�V����(�J�����ɏ����Ƃ�)
	LateUpdate();
	sceneObjects->LateUpdate();

	// �V�[�����̓����蔻����`�F�b�N����
	/*collisionChcker->CollisionCheck();*/

	// ��ʃN���A�ȂǏ���
	Direct3D11::GetInstance()->GetRenderer()->SetUpDraw();
	// �r���[�ϊ��s����X�V
	Camera* mainCamera = sceneObjects->GetSceneObject<Camera>("MainCamera");
	mainCamera->UpdateViewMatrix();
	// �V�[�����̕`�揈��
	Draw();
	sceneObjects->Draw();
	
}

//#include "pch.h"
//#include "nSceneManager.h"
//#include "SceneManager.h"
//#include "BroadScene_Base.h"
//#include "ChangeBroadScene.h"
//#include "Test_ChangeSubScene.h"
//#include "Tank_ChangeSub.h"
//#include "SceneMoveInfo.h"
//
//#include "ShaderCollection.h"
//
//#include "InSceneSystemManager.h"
//
//#include "GameInput.h"
//
//// �A�Z�b�g������
//#include "AssetSetter.h"
//#include "Geometory.h"
//#include "Material.h"
//
//
//
//SceneManager* SceneManager::pInstance = nullptr;	// �C���X�^���X�̏�����
//
//SceneManager::SceneManager()
//{
//	Setup();
//
//	// �����V�[���̏��
//	int initSub = Tank_ChangeSub::Scene::InGame;
//	BroadType::Type initBroad = BroadType::Tank;
//
//	// �V�[���J�ڏ����m��(�����V�[������������)
//	pMoveInfo = new SceneMoveInfo(initSub, initBroad);
//
//	// ��ǃV�[���ϊ��N���X���m��
//	pChaneBroad = new ChangeBroadScene(pMoveInfo);
//	// �����V�[���ɑJ�ڂ���
//	pNowBroadScene = pChaneBroad->OnChangeBroad();
//}
//
//SceneManager::~SceneManager()
//{
//	Release();
//}
//
//void SceneManager::Setup()
//{
//	// �A�Z�b�g�֘A
//	AssetSetup();
//
//	// �C�[�W���O������
//	HashiTaku::Easing::Init();
//}
//
//void SceneManager::AssetSetup()
//{
//	// �f�t�H�ł���}�e���A�����쐬
//	MaterialSetup();
//
//	// ��{�I�u�W�F�N�g�̏�����
//	Geometory::Init();
//}
//
//void SceneManager::CheckChangeBroad()
//{
//	// �ғ���ԏI������Ȃ��Ȃ珈���͒ʂ�Ȃ�
//	if (pMoveInfo->GetRunningState() != SceneRunningState::RUNNING_STATE::FINISH)
//		return;
//
//	// ���ғ��I���Ȃ�
//	pNowBroadScene = pChaneBroad->OnChangeBroad();
//}
//
//void SceneManager::Release()
//{
//	CLASS_DELETE(pChaneBroad);
//	CLASS_DELETE(pMoveInfo);
//
//	Geometory::Release();
//	InSceneSystemManager::Delete();
//	GameInput::Delete();
//}
//
//void SceneManager::Exec()
//{
//	// �Q�[�������͍X�V
//	GameInput::GetInstance()->Update();
//
//	// ��ǃV�[���̎��s
//	pNowBroadScene->Exec();
//
//	// �V�[���J�ڂ��邩�ǂ����m�F
//	CheckChangeBroad();
//}
//
//void SceneManager::MaterialSetup()
//{
//	// Unlit�}�e���A���쐬
//	std::unique_ptr<Material> pUnlit = std::make_unique<Material>();
//	AssetSetter::SetAsset("M_Unlit", std::move(pUnlit));
//}

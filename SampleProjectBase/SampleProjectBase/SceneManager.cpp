#include "SceneManager.h"
#include "BroadScene_Base.h"
#include "ChangeBroadScene.h"
#include "Test_ChangeSubScene.h"
#include "Tank_ChangeSub.h"
#include "SceneMoveInfo.h"
#include "ShaderCollection.h"
#include "Geometory.h"
#include "CollisionRegister.h"

SceneManager* SceneManager::pInstance = nullptr;	// �C���X�^���X�̏�����

SceneManager::SceneManager()
{
	// �V�F�[�_�[�Ǘ��N���X������
	ShaderCollection* shCollection = ShaderCollection::GetInstance();
	shCollection->Init();

	Geometory::Init();	// ��{�I�u�W�F�N�g�̏�����

	// �����V�[���̏��
	int initSub = Tank_ChangeSub::Scene::InGame;
	BroadType::Type initBroad = BroadType::Tank;

	// �V�[���J�ڏ����m��(�����V�[������������)
	pMoveInfo = new SceneMoveInfo(initSub ,initBroad);

	// ��ǃV�[���ϊ��N���X���m��
	pChaneBroad = new ChangeBroadScene(pMoveInfo);
	// �����V�[���ɑJ�ڂ���
	pNowBroadScene = pChaneBroad->OnChangeBroad();
}

SceneManager::~SceneManager()
{
	Release();
}

void SceneManager::CheckChangeBroad()
{
	// �ғ���ԏI������Ȃ��Ȃ珈���͒ʂ�Ȃ�
	if (pMoveInfo->GetRunningState() != SceneRunningState::RUNNING_STATE::FINISH)
		return;

	// ���ғ��I���Ȃ�
	pNowBroadScene = pChaneBroad->OnChangeBroad();
}

void SceneManager::Release()
{
	CLASS_DELETE(pChaneBroad);
	CLASS_DELETE(pMoveInfo);

	Geometory::Release();
	ResourceCollection::Delete();
	ShaderCollection::Delete();
	CollisionRegister::Delete();
}

void SceneManager::Exec()
{
	// ��ǃV�[���̎��s
	pNowBroadScene->Exec();

	// �V�[���J�ڂ��邩�ǂ����m�F
	CheckChangeBroad();
}

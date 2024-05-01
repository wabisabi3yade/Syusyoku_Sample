#include "SceneManager.h"
#include "BroadScene_Base.h"
#include "ChangeBroadScene.h"
#include "Test_ChangeSubScene.h"
#include "SceneMoveInfo.h"
SceneManager* SceneManager::pInstance = nullptr;	// �C���X�^���X�̏�����

SceneManager::SceneManager()
{
	// �����V�[���̏��
	int initSub = Test_ChangeSubScene::TEST1;
	BROAD_TYPE::TYPE initBroad = BROAD_TYPE::TEST;

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
}

void SceneManager::Exec()
{
	// ��ǃV�[���̎��s
	pNowBroadScene->Exec();

	// �V�[���J�ڂ��邩�ǂ����m�F
	CheckChangeBroad();
}

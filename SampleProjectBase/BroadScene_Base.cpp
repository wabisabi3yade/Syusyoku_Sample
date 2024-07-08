#include "BroadScene_Base.h"
#include "SubScene_Base.h"
#include "SceneMoveInfo.h"
#include "ChangeSubScene.h"

BroadScene_Base::BroadScene_Base(SceneMoveInfo* _pMoveInfo)
{
	
}

BroadScene_Base::~BroadScene_Base()
{
}

SubScene_Base* BroadScene_Base::ChangeSubScene()
{
	// �T�u�V�[����J��
	pNowScene = pChangeSubScene->ChangeScene();

	// �J�ڐ�̃T�u�V�[���̃|�C���^��Ԃ�
	return pNowScene;
}

void BroadScene_Base::Exec()
{
	// �T�u�V�[���̎��s�֐�
	pNowScene->Exec();
}

void BroadScene_Base::Release()
{
}

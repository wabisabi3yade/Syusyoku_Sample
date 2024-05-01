#include "ChangeBroadScene.h"
#include "SceneMoveInfo.h"
#include "Test_Broad.h"
#include "Test_ChangeSubScene.h"

ChangeBroadScene::ChangeBroadScene(SceneMoveInfo* _moveInfo)
{
	pMoveInfo = _moveInfo;	// �V�[���J�ڏ�����
	c_BroadId = BROAD_TYPE::NONE;	// �����l�i�ǂ̃V�[���ɂ����Ȃ��l�ɂ��Ă����j
}

ChangeBroadScene::~ChangeBroadScene()
{
	CLASS_DELETE(pBroadScene);
}

BroadScene_Base* ChangeBroadScene::OnChangeBroad()
{
	// ���̑�ǃV�[��Id���擾
	const BROAD_TYPE::TYPE nextBroad = pMoveInfo->GetNextBroadType();

	// ���݂̑�ǂƑJ�ڐ�̑�ǂ������Ȃ�
	if (c_BroadId == nextBroad)
	{
		// �T�u�V�[���������@�ς���
		pBroadScene->ChangeSubScene();
		return pBroadScene;
	}

	// �����݂̑�ǂƑJ�ڐ�̑�ǂ��Ⴄ�Ȃ�

	// ��ǃV�[�������
	CLASS_DELETE(pBroadScene);	
	// ��ǃV�[����ύX����
	switch (nextBroad)
	{
	case BROAD_TYPE::TEST:
		pBroadScene = new Test_Broad(pMoveInfo);
		break;
	default:
		
		break;
	}
	// �J�ڐ�̑��Id��ۑ����Ă���
	c_BroadId = nextBroad;

	return pBroadScene;
}

void ChangeBroadScene::Release()
{
	CLASS_DELETE(pBroadScene);
}

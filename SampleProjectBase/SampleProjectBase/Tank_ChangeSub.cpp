#include "pch.h"
#include "Tank_ChangeSub.h"
// �T�u�V�[��
#include "Tank_TitleSub.h"
#include "Tank_InGameSub.h"
#include "Tank_ResultSub.h"

Tank_ChangeSub::Tank_ChangeSub(SceneMoveInfo* _moveInfo) : ChangeSubScene(_moveInfo)
{
}

Tank_ChangeSub::~Tank_ChangeSub()
{
}

SubScene_Base* Tank_ChangeSub::ChangeScene()
{
	// ��ɑO�̉������
	CLASS_DELETE(pSubScene);

	// ���̃T�u�V�[����id���擾
	int nextSubId = moveInfo->GetNextSubType();

	// �J�ڐ�̃T�u�V�[���ɑΉ������V�[�����쐬����
	switch (nextSubId)
	{
	case Scene::Title:
		pSubScene = new Tank_TitleSub(moveInfo);
		break;

	case Scene::InGame:
		pSubScene = new Tank_InGameSub(moveInfo);
		break;

	case Scene::Result:
		pSubScene = new Tank_ResultSub(moveInfo);
		break;

	default:
		break;
	}

	return pSubScene;
}

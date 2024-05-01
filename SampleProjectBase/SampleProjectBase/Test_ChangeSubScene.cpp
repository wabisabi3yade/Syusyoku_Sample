#include "Test_ChangeSubScene.h"
#include "SceneMoveInfo.h"
#include "Test_SubScene.h"

Test_ChangeSubScene::Test_ChangeSubScene(SceneMoveInfo* _moveInfo)
	: ChangeSubScene(_moveInfo)
{
}

Test_ChangeSubScene::~Test_ChangeSubScene()
{
}

SubScene_Base* Test_ChangeSubScene::ChangeScene()
{
	// ��ɑO�̉������
	CLASS_DELETE(pSubScene);

	// ���̃T�u�V�[����id���擾
	int nextSubId = moveInfo->GetNextSubType();

	// �J�ڐ�̃T�u�V�[���ɑΉ������V�[�����쐬����
	switch (nextSubId)
	{
	case TEST_SCENE::TEST1:
		pSubScene = new Test_SubScene(moveInfo);
		break;

	case TEST_SCENE::TEST2:
		break;

	case TEST_SCENE::TEST3:
		break;

	default:
		break;
	}

	return pSubScene;
}

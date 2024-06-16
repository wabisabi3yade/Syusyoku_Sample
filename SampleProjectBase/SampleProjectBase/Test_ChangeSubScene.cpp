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
	case Scene::Test1:
		pSubScene = new Test_SubScene(moveInfo);
		break;

	case Scene::Test2:
		break;

	case Scene::Test3:
		break;

	default:
		break;
	}

	return pSubScene;
}

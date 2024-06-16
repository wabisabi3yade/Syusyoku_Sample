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
	// 先に前の解放する
	CLASS_DELETE(pSubScene);

	// 次のサブシーンのidを取得
	int nextSubId = moveInfo->GetNextSubType();

	// 遷移先のサブシーンに対応したシーンを作成する
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

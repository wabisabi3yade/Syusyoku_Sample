#include "Test_Broad.h"
#include "Test_ChangeSubScene.h"
#include "SceneMoveInfo.h"

Test_Broad::Test_Broad(SceneMoveInfo* _pMoveInfo)
	: BroadScene_Base(_pMoveInfo)
{
	// シーン遷移クラスの初期化
	InitChangeScene(_pMoveInfo);
}

Test_Broad::~Test_Broad()
{
	Release();
}

void Test_Broad::InitChangeScene(SceneMoveInfo* _moveInfo)
{
	pChangeSubScene = new Test_ChangeSubScene(_moveInfo);

	pNowScene = pChangeSubScene->ChangeScene();
}

void Test_Broad::Release()
{
	CLASS_DELETE(pChangeSubScene);
}

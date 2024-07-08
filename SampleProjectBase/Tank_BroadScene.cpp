#include "pch.h"
#include "Tank_BroadScene.h"
#include "Tank_ChangeSub.h"

void Tank_BroadScene::InitChangeScene(SceneMoveInfo* _moveInfo) 
{
	pChangeSubScene = new Tank_ChangeSub(_moveInfo);

	pNowScene = pChangeSubScene->ChangeScene();
}

Tank_BroadScene::Tank_BroadScene(SceneMoveInfo* _pMoveInfo) : BroadScene_Base(_pMoveInfo)
{
	// シーン遷移クラスの初期化
	InitChangeScene(_pMoveInfo);
}

Tank_BroadScene::~Tank_BroadScene()
{
	CLASS_DELETE(pChangeSubScene);
}

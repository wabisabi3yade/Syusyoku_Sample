#include "ChangeSubScene.h"


ChangeSubScene::ChangeSubScene(SceneMoveInfo* _moveInfo)
{
	pSubScene = nullptr;
	moveInfo = _moveInfo;
}

ChangeSubScene::~ChangeSubScene()
{
	CLASS_DELETE(pSubScene);
}

SubScene_Base* ChangeSubScene::ChangeScene()
{
	return nullptr;
}
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
	// サブシーンを遷移
	pNowScene = pChangeSubScene->ChangeScene();

	// 遷移先のサブシーンのポインタを返す
	return pNowScene;
}

void BroadScene_Base::Exec()
{
	// サブシーンの実行関数
	pNowScene->Exec();
}

void BroadScene_Base::Release()
{
}

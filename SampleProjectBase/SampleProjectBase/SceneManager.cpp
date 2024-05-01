#include "SceneManager.h"
#include "BroadScene_Base.h"
#include "ChangeBroadScene.h"
#include "Test_ChangeSubScene.h"
#include "SceneMoveInfo.h"
SceneManager* SceneManager::pInstance = nullptr;	// インスタンスの初期化

SceneManager::SceneManager()
{
	// 初期シーンの情報
	int initSub = Test_ChangeSubScene::TEST1;
	BROAD_TYPE::TYPE initBroad = BROAD_TYPE::TEST;

	// シーン遷移情報を確保(初期シーン情報を引数に)
	pMoveInfo = new SceneMoveInfo(initSub ,initBroad);

	// 大局シーン変換クラスを確保
	pChaneBroad = new ChangeBroadScene(pMoveInfo);
	// 初期シーンに遷移する
	pNowBroadScene = pChaneBroad->OnChangeBroad();
}

SceneManager::~SceneManager()
{
	Release();
}

void SceneManager::CheckChangeBroad()
{
	// 稼働状態終了じゃないなら処理は通らない
	if (pMoveInfo->GetRunningState() != SceneRunningState::RUNNING_STATE::FINISH)
		return;

	// ↓稼働終了なら
	pNowBroadScene = pChaneBroad->OnChangeBroad();
}

void SceneManager::Release()
{
	CLASS_DELETE(pChaneBroad);
	CLASS_DELETE(pMoveInfo);
}

void SceneManager::Exec()
{
	// 大局シーンの実行
	pNowBroadScene->Exec();

	// シーン遷移するかどうか確認
	CheckChangeBroad();
}

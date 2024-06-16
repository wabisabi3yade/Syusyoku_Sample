#include "ChangeBroadScene.h"
#include "SceneMoveInfo.h"
#include "Test_Broad.h"
#include "Test_ChangeSubScene.h"
#include "Tank_BroadScene.h"

ChangeBroadScene::ChangeBroadScene(SceneMoveInfo* _moveInfo)
{
	pMoveInfo = _moveInfo;	// シーン遷移情報を代入
	c_BroadId = BroadType::None;	// 初期値（どのシーンにも被らない値にしておく）
}

ChangeBroadScene::~ChangeBroadScene()
{
	CLASS_DELETE(pBroadScene);
}

BroadScene_Base* ChangeBroadScene::OnChangeBroad()
{
	// 次の大局シーンIdを取得
	const BroadType::Type nextBroad = pMoveInfo->GetNextBroadType();

	// 現在の大局と遷移先の大局が同じなら
	if (c_BroadId == nextBroad)
	{
		// サブシーンだけを　変える
		pBroadScene->ChangeSubScene();
		return pBroadScene;
	}

	// ↓現在の大局と遷移先の大局が違うなら

	// 大局シーンを解放
	CLASS_DELETE(pBroadScene);	
	// 大局シーンを変更する
	switch (nextBroad)
	{
	case BroadType::Test:
		pBroadScene = new Test_Broad(pMoveInfo);
		break;

	case BroadType::Tank:
		pBroadScene = new Tank_BroadScene(pMoveInfo);
		break;

	default:
		break;
	}
	// 遷移先の大局Idを保存しておく
	c_BroadId = nextBroad;

	return pBroadScene;
}

void ChangeBroadScene::Release()
{
	CLASS_DELETE(pBroadScene);
}

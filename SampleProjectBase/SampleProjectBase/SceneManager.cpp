#include "SceneManager.h"
#include "BroadScene_Base.h"
#include "ChangeBroadScene.h"
#include "Test_ChangeSubScene.h"
#include "Tank_ChangeSub.h"
#include "SceneMoveInfo.h"
#include "ShaderCollection.h"
#include "Geometory.h"
#include "CollisionRegister.h"

SceneManager* SceneManager::pInstance = nullptr;	// インスタンスの初期化

SceneManager::SceneManager()
{
	// シェーダー管理クラス初期化
	ShaderCollection* shCollection = ShaderCollection::GetInstance();
	shCollection->Init();

	Geometory::Init();	// 基本オブジェクトの初期化

	// 初期シーンの情報
	int initSub = Tank_ChangeSub::Scene::InGame;
	BroadType::Type initBroad = BroadType::Tank;

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

	Geometory::Release();
	ResourceCollection::Delete();
	ShaderCollection::Delete();
	CollisionRegister::Delete();
}

void SceneManager::Exec()
{
	// 大局シーンの実行
	pNowBroadScene->Exec();

	// シーン遷移するかどうか確認
	CheckChangeBroad();
}

#include "SubScene_Base.h"
#include "SceneMoveInfo.h"

// カメラ
#include "CP_Camera.h"

// システム系
#include "GameMode.h"
#include "InSceneSystemManager.h"

#include "ImGuiMethod.h"

void SubScene_Base::DrawSetup()
{
	SceneLights& sceneLights = systemManager->GetSceneLights();

	// 画面クリアなど準備
	Direct3D11::GetInstance()->GetRenderer()->SetUpDraw();

	// 光源の更新処理
	sceneLights.Update();

	// 1ループ1度だけ更新すればいいバッファ更新
	ShaderCollection* shCol = ShaderCollection::GetInstance();
	shCol->UniqueUpdateBuffer();
}

/// <summary>
/// シーン移動するとなったら呼び出す
/// </summary>
/// <param name="_nextSubType">次のサブシーンのタイプ</param>
/// <param name="_nextBroadType">次の大局シーンのタイプ</param>
void SubScene_Base::OnMoveScene(int _nextSubType, BroadType::Type _nextBroadType)
{
	// シーン遷移情報を遷移先に変える
	pSceneMoveInfo->OnChangeScene(_nextSubType, _nextBroadType);
}

SubScene_Base::SubScene_Base(SceneMoveInfo* _pSceneMoveInfo)
{
	pSceneMoveInfo = _pSceneMoveInfo;

	// システム初期化
	systemManager = InSceneSystemManager::GetInstance();
	systemManager->Init();
}

SubScene_Base::~SubScene_Base()
{
	// シーンのシステムを破棄
	systemManager->Delete();

	// シーンごとにゲームモードを作り直す
	GameMode::Delete();
}


void SubScene_Base::Exec()
{
	SceneObjects& sceneObjects = systemManager->GetSceneObjects();
	CollisionChecker& collisionChecker = systemManager->GetCollisonChecker();

	// シーン内の更新処理
	Update();
	sceneObjects.Update();

	// Updateの後に行う更新処理(カメラに処理とか)
	LateUpdate();
	sceneObjects.LateUpdate();

	// シーン内の当たり判定をチェックする
	collisionChecker.CollisionCheck();

	// 描画前準備
	DrawSetup();

	// シーン内の描画処理
	Draw();
	sceneObjects.Draw();
	
}

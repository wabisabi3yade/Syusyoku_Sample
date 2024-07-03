#include "SubScene_Base.h"
#include "SceneMoveInfo.h"

// カメラ
#include "Camera.h"

// システム系
#include "SObjectRegister.h"
#include "CollisionRegister.h"
#include "GameMode.h"
#include "InSceneSystemManager.h"


#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"
#include "ImGuiMethod.h"

void SubScene_Base::DrawSetup()
{
	// 画面クリアなど準備
	Direct3D11::GetInstance()->GetRenderer()->SetUpDraw();

	// ビュー変換行列を更新
	Camera* mainCamera = sceneObjects->GetSceneObject<Camera>("MainCamera");
	mainCamera->UpdateViewMatrix();

	// 光源の更新処理
	sceneLights->Update();

	resourceCollection->ImportDisplay();
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
	resourceCollection = ResourceCollection::GetInstance();

	// インスタンス生成
	sceneObjects = std::make_unique<SceneObjects>();
	sceneLights = std::make_unique<SceneLights>();
	collisionChecker = std::make_unique<CollisionChecker>();
	std::unique_ptr<Camera> mainCamera = std::make_unique<Camera>();

	// シーンオブジェクト登録クラスセット
	SObjectRegister::GetInstance()->SetSceneObjects(*sceneObjects.get());

	// 当たり判定チェッカークラスを生成し、当たり判定登録対象にする
	CollisionRegister::GetInstance()->SetCollisionChecker(*collisionChecker);

	// 各インスタンスを渡す
	InSceneSystemManager* system = InSceneSystemManager::GetInstance();
	system->SetSceneObjects(*sceneObjects.get());
	system->SetSceneLights(*sceneLights.get());
	system->SetCollisonChecker(*collisionChecker);

	// カメラ設定
	Camera* camPtr = mainCamera.get();
	GameMode::GetInstance()->SetCamera(*camPtr);	// ゲームモードにカメラを設定する
	sceneObjects->SetObject("MainCamera", std::move(mainCamera));
}

SubScene_Base::~SubScene_Base()
{
	// シーン内のオブジェクトの終了処理を行う
	sceneObjects.reset();	

	// シーンごとにゲームモードを作り直す
	GameMode::Delete();

	// 登録クラスを消しておく
	CollisionRegister::Delete();
	SObjectRegister::Delete();
}


void SubScene_Base::Exec()
{
	// シーン内の更新処理
	Update();
	sceneObjects->Update();

	// Updateの後に行う更新処理(カメラに処理とか)
	LateUpdate();
	sceneObjects->LateUpdate();

	// シーン内の当たり判定をチェックする
	collisionChecker->CollisionCheck();

	// 描画前準備
	DrawSetup();

	// シーン内の描画処理
	Draw();
	sceneObjects->Draw();
	
}

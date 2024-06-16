#include "SubScene_Base.h"
#include "SceneMoveInfo.h"

#include "Camera.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"
#include "ImGuiMethod.h"

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
	// BroadSceneから持ってくる
	pSceneMoveInfo = _pSceneMoveInfo;
	// リソース管理ポインタ取得
	resourceCollection = ResourceCollection::GetInstance();
	// シーンオブジェクト管理クラス作成
	sceneObjects = std::make_unique<SceneObjects>();
	// カメラ生成
	std::unique_ptr<Camera> mainCamera = std::make_unique<Camera>();
	sceneObjects->SetObject("mainCamera", std::move(mainCamera));
}

SubScene_Base::~SubScene_Base()
{
}


void SubScene_Base::Exec()
{
	// シーン内の更新処理
	Update();
	sceneObjects->Update();
	// Updateの後に行う更新処理(カメラに処理とか)
	LateUpdate();
	sceneObjects->LateUpdate();
	// 画面クリアなど準備
	Direct3D11::GetInstance()->GetRenderer()->SetUpDraw();
	// ビュー変換行列を更新
	Camera* mainCamera = sceneObjects->GetSceneObject<Camera>("mainCamera");
	mainCamera->UpdateViewMatrix();

	// シーン内の描画処理
	Draw();
	sceneObjects->Draw();
	ImGuiMethod::Draw();

	// スワップチェイン
	Direct3D11::GetInstance()->GetRenderer()->Swap();
}

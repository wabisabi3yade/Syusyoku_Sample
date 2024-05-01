#include "SubScene_Base.h"
#include "SceneMoveInfo.h"

#include "Direct3D11.h"
#include "Camera.h"

/// <summary>
/// シーン移動するとなったら呼び出す
/// </summary>
/// <param name="_nextSubType">次のサブシーンのタイプ</param>
/// <param name="_nextBroadType">次の大局シーンのタイプ</param>
void SubScene_Base::OnMoveScene(int _nextSubType, BROAD_TYPE::TYPE _nextBroadType)
{
	// シーン遷移情報を遷移先に変える
	pSceneMoveInfo->OnChangeScene(_nextSubType, _nextBroadType);
}

SubScene_Base::SubScene_Base(SceneMoveInfo* _pSceneMoveInfo)
{
	// BroadSceneから持ってくる
	pSceneMoveInfo = _pSceneMoveInfo;
	// カメラ生成
	mainCamera = std::make_unique<Camera>();
}

SubScene_Base::~SubScene_Base()
{
}


void SubScene_Base::Exec()
{
	// シーン内の更新処理
	Update();
	// Updateの後に行う更新処理(カメラに処理とか)
	LateUpdate();

	// 画面クリア
	Direct3D11::GetInstance()->GetRenderer()->SetUpDraw();

	// ビュー変換行列を求める
	DirectX::XMMATRIX viewMatrix = mainCamera->GetViewMatrix();
	Direct3D11::GetInstance()->GetRenderer()->SetUpViewTransform(viewMatrix);

	// シーン内の描画処理
	Draw();
	// スワップチェイン
	Direct3D11::GetInstance()->GetRenderer()->Swap();
}

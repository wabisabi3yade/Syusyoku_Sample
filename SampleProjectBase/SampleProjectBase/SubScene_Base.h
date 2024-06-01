#pragma once
#include "BroadSceneType.h"
#include "Camera.h"
#include "ModelLoader.h"

class SceneMoveInfo;

// サブシーン（小さなシーン、例えば街の中の家の中とか）の基底クラス
class SubScene_Base
{
	// シーン遷移の情報（BroadSceneから代入する）
	SceneMoveInfo* pSceneMoveInfo;

protected:
	ResourceCollection* resourceCollection;	// リソース管理ポインタ
	std::unique_ptr<Camera> mainCamera;	// メインカメラオブジェクト

	// 更新処理
	virtual void Update() = 0;

	// Updateのあとに行われる更新処理
	virtual void LateUpdate() = 0;

	// 描画をする
	virtual void Draw() = 0;

	// シーン移動で呼び出す
	void OnMoveScene(int _nextSubType, BROAD_TYPE::TYPE _nextBroadType);
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pSceneMoveInfo">大局クラスから受け取ったシーン遷移情報</param>
	SubScene_Base(SceneMoveInfo* _pSceneMoveInfo);
	virtual ~SubScene_Base();

	// シーン実行処理
	void Exec();

	// 解放処理
	virtual void Release() = 0;
};


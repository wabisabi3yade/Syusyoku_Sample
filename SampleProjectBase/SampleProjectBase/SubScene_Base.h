#pragma once
#include "SceneMoveInfo.h"

// オブジェクト
#include "Camera.h"
#include "Object_3D.h"
#include "Object_2D.h"
#include "Object_UI.h"

// システム関連
#include "SceneObjects.h"
#include "SceneLights.h"
#include "CollisionChecker.h"
#include "ModelLoader.h"
#include "ShaderCollection.h"

// サブシーン（小さなシーン、例えば街の中の家の中とか）の基底クラス
class SubScene_Base
{
	// シーン遷移の情報（BroadSceneから代入する）
	SceneMoveInfo* pSceneMoveInfo;

protected:
	// リソース管理
	ResourceCollection* resourceCollection;	

	// シーン内オブジェクト管理
	std::unique_ptr<SceneObjects> sceneObjects;

	// シーン内光源
	std::unique_ptr<SceneLights> sceneLights;

	// 当たり判定のチェック
	std::unique_ptr<CollisionChecker> collisionChecker;	

	// 更新処理
	virtual void Update() = 0;

	// Updateのあとに行われる更新処理
	virtual void LateUpdate() = 0;

	// 描画をする
	virtual void Draw() = 0;
	void DrawSetup();

	// シーン移動で呼び出す
	void OnMoveScene(int _nextSubType, BroadType::Type _nextBroadType);
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
	virtual void Release() {};
};


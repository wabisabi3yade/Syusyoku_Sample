#pragma once
#include "SceneMoveInfo.h"

// システム関連
#include "AssetLoader.h"
#include "ShaderCollection.h"
#include "InSceneSystemManager.h"

// アセット
#include "StaticMesh.h"
#include "SkeletalMesh.h"

class SceneObjects;
class CollisionChecker;

// サブシーン（小さなシーン、例えば街の中の家の中とか）の基底クラス
class SubScene_Base
{
	// シーン遷移の情報（BroadSceneから代入する）
	SceneMoveInfo* pSceneMoveInfo;

protected:
	//	シーン内のシステムマネジャー
	InSceneSystemManager* systemManager;

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


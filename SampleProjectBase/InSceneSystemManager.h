#pragma once

// シーン内システムクラス
#include "SceneObjects.h"
#include "SceneLights.h"
#include "CollisionChecker.h"

// シーン内部のシステムのインスタンスを持つクラス
class InSceneSystemManager : public Singleton_Base<InSceneSystemManager>
{
	friend class Singleton_Base<InSceneSystemManager>;

	InSceneSystemManager(){};
	~InSceneSystemManager();

	// インスタンス
	// シーンオブジェクト
	std::unique_ptr<SceneObjects> pSceneObjects;

	// シーンライティング
	std::unique_ptr<SceneLights> pSceneLights;

	// 衝突判定
	std::unique_ptr<CollisionChecker> pCollisionChecker;
public:
	void Init();

	SceneObjects& GetSceneObjects() { return *pSceneObjects; }
	SceneLights& GetSceneLights() { return *pSceneLights; }
	CollisionChecker& GetCollisonChecker() { return *pCollisionChecker; }
};


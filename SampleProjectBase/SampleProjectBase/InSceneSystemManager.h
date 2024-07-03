#pragma once

// シーン内システムクラス
#include "SceneObjects.h"
#include "SceneLights.h"
#include "CollisionChecker.h"

// シーン内部のシステムのインスタンスを持つクラス
class InSceneSystemManager : public Singleton_Base<InSceneSystemManager>
{
	friend class Singleton_Base<InSceneSystemManager>;

	InSceneSystemManager() : pSceneObjects(nullptr), pSceneLights(nullptr), pCollisionChecker(nullptr) {};
	~InSceneSystemManager(){};

	// インスタンス
	SceneObjects* pSceneObjects;
	SceneLights* pSceneLights;
	CollisionChecker* pCollisionChecker;
public:

	void SetSceneObjects(SceneObjects& _sceneObjects) { pSceneObjects = &_sceneObjects; }
	void SetSceneLights(SceneLights& _sceneLights) { pSceneLights = &_sceneLights; }
	void SetCollisonChecker(CollisionChecker& _collisionChecker) { pCollisionChecker = &_collisionChecker; }

	SceneObjects& GetSceneObjects() { return *pSceneObjects; }
	SceneLights& GetSceneLights() { return *pSceneLights; }
	CollisionChecker& GetCollisonChecker() { return *pCollisionChecker; }
};


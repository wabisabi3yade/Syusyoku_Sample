#include "pch.h"
#include "InSceneSystemManager.h"

InSceneSystemManager::~InSceneSystemManager()
{
	// シーンオブジェクトだけ先に解放する
	pSceneObjects.reset();
}

void InSceneSystemManager::Init()
{
	pSceneObjects = std::make_unique<SceneObjects>();
	pSceneLights = std::make_unique<SceneLights>();
	pCollisionChecker = std::make_unique<CollisionChecker>();
}

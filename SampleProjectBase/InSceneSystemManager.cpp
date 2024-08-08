#include "pch.h"
#include "InSceneSystemManager.h"

// カメラ
#include "Camera.h"

using namespace SceneFunction;

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

	// メインカメラを作成する
	CreateMainCamera();
}

CP_Camera& InSceneSystemManager::GetMainCamera()
{
	return *pMainCamera;
}

SceneObjects& InSceneSystemManager::GetSceneObjects()
{
	return *pSceneObjects;
}

SceneLights& InSceneSystemManager::GetSceneLights()
{
	return *pSceneLights;
}

void InSceneSystemManager::CreateMainCamera()
{
	GameObject& cameraObject = ObjectFunc::CreateEmpty("MainCamera");
	pMainCamera = cameraObject.AddComponent<CP_Camera>();
}

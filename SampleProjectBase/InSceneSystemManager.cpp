#include "pch.h"
#include "InSceneSystemManager.h"

// カメラ
#include "CP_Camera.h"

using namespace SceneFunction;

InSceneSystemManager::~InSceneSystemManager()
{
	pSceneObjects.reset();
}

void InSceneSystemManager::Init()
{
	pSceneObjects = std::make_unique<SceneObjects>();
	pSceneLights = std::make_unique<SceneLights>();
}

void InSceneSystemManager::Reset()
{
	pMainCamera = nullptr;

	// シーンオブジェクトだけ先に解放する
	pSceneObjects.reset();
	pSceneLights.reset();

	pSceneObjects = std::make_unique<SceneObjects>();
	pSceneLights = std::make_unique<SceneLights>();
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

void InSceneSystemManager::SetCamera(CP_Camera& _camera)
{
	pMainCamera = &_camera;
}

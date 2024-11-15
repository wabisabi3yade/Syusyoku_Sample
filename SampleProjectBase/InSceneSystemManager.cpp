#include "pch.h"
#include "InSceneSystemManager.h"

// カメラ
#include "CP_Camera.h"

using namespace SceneFunction;

InSceneSystemManager::~InSceneSystemManager()
{
	pSceneObjects.reset();

	if (isNullCamera)	// nullカメラなら
		CLASS_DELETE(pMainCamera);
}

void InSceneSystemManager::Init()
{
	// シーンオブジェクトを作成
	pSceneObjects = std::make_unique<SceneObjects>();

	// シーン光源作成
	pSceneLights = std::make_unique<SceneLights>();

	pGameInput = std::make_unique<GameInput>();

	// Nullオブジェクトを作成
	pMainCamera = new CP_CameraNull();
	isNullCamera = true;
}

void InSceneSystemManager::InputUpdate()
{
	// ゲーム内入力更新
	pGameInput->Update();
}

void InSceneSystemManager::Reset()
{
	if (!isNullCamera)
	{
		pMainCamera = new CP_CameraNull();
		isNullCamera = true;
	}
		
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

void InSceneSystemManager::UpdateViewMatrix()
{
	if (pMainCamera)
		pMainCamera->UpdateViewMatrix();
}

SceneObjects& InSceneSystemManager::GetSceneObjects()
{
	return *pSceneObjects;
}

SceneLights& InSceneSystemManager::GetSceneLights()
{
	return *pSceneLights;
}

GameInput& InSceneSystemManager::GetInput()
{
	return *pGameInput;
}

void InSceneSystemManager::SetCamera(CP_Camera& _camera)
{
	if (isNullCamera)	// nullオブジェクトを持っているなら
		CLASS_DELETE(pMainCamera);

	pMainCamera = &_camera;
	isNullCamera = false;
}

void InSceneSystemManager::DeleteCamera(CP_Camera& _camera)
{
	if (pMainCamera != &_camera) return;

	// nullカメラオブジェクトを作成
	pMainCamera = new CP_CameraNull();
	isNullCamera = true;
}

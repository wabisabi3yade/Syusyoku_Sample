#include "pch.h"
#include "InSceneSystemManager.h"

// �J����
#include "CP_Camera.h"

using namespace SceneFunction;

InSceneSystemManager::~InSceneSystemManager()
{
	// �V�[���I�u�W�F�N�g������ɉ������
	pSceneObjects.reset();
}

void InSceneSystemManager::Init()
{
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

#include "pch.h"
#include "InSceneSystemManager.h"

// �J����
#include "CP_Camera.h"

using namespace SceneFunction;

InSceneSystemManager::~InSceneSystemManager()
{
	pSceneObjects.reset();

	if (isNullCamera)	// null�J�����Ȃ�
		CLASS_DELETE(pMainCamera);
}

void InSceneSystemManager::Init()
{
	// �V�[���I�u�W�F�N�g���쐬
	pSceneObjects = std::make_unique<SceneObjects>();

	// �V�[�������쐬
	pSceneLights = std::make_unique<SceneLights>();

	pGameInput = std::make_unique<GameInput>();

	// Null�I�u�W�F�N�g���쐬
	pMainCamera = new CP_CameraNull();
	isNullCamera = true;
}

void InSceneSystemManager::InputUpdate()
{
	// �Q�[�������͍X�V
	pGameInput->Update();
}

void InSceneSystemManager::Reset()
{
	if (!isNullCamera)
	{
		pMainCamera = new CP_CameraNull();
		isNullCamera = true;
	}
		
	// �V�[���I�u�W�F�N�g������ɉ������
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
	if (isNullCamera)	// null�I�u�W�F�N�g�������Ă���Ȃ�
		CLASS_DELETE(pMainCamera);

	pMainCamera = &_camera;
	isNullCamera = false;
}

void InSceneSystemManager::DeleteCamera(CP_Camera& _camera)
{
	if (pMainCamera != &_camera) return;

	// null�J�����I�u�W�F�N�g���쐬
	pMainCamera = new CP_CameraNull();
	isNullCamera = true;
}

#include "pch.h"
#include "InSceneSystemManager.h"

// �J����
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

void InSceneSystemManager::AddRigidBody(CP_RigidBody2& _setRb)
{
#ifdef _DEBUG
	auto itr = std::find(pSceneRbs.begin(), pSceneRbs.end(), &_setRb);
	if (itr != pSceneRbs.end())
	{
		HASHI_DEBUG_LOG("RigidBody���d�����Ă��܂�");
		return;
	}
#endif // _DEBUG

	pSceneRbs.push_back(&_setRb);
}

void InSceneSystemManager::RemoveRigidBody(CP_RigidBody2& _removetRb)
{
	pSceneRbs.remove(&_removetRb);
}

void InSceneSystemManager::UpdateTransformBtToDx()
{
	for (auto& pRb : pSceneRbs)
		pRb->SetToDXTransform();
}

#include "pch.h"
#include "InSceneSystemManager.h"

InSceneSystemManager::~InSceneSystemManager()
{
	// �V�[���I�u�W�F�N�g������ɉ������
	pSceneObjects.reset();
}

void InSceneSystemManager::Init()
{
	pSceneObjects = std::make_unique<SceneObjects>();
	pSceneLights = std::make_unique<SceneLights>();
	pCollisionChecker = std::make_unique<CollisionChecker>();
}

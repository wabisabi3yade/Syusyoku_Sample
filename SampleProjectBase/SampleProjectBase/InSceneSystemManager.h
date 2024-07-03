#pragma once

// �V�[�����V�X�e���N���X
#include "SceneObjects.h"
#include "SceneLights.h"
#include "CollisionChecker.h"

// �V�[�������̃V�X�e���̃C���X�^���X�����N���X
class InSceneSystemManager : public Singleton_Base<InSceneSystemManager>
{
	friend class Singleton_Base<InSceneSystemManager>;

	InSceneSystemManager() : pSceneObjects(nullptr), pSceneLights(nullptr), pCollisionChecker(nullptr) {};
	~InSceneSystemManager(){};

	// �C���X�^���X
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


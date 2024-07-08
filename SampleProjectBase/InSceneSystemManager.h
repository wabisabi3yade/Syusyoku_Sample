#pragma once

// �V�[�����V�X�e���N���X
#include "SceneObjects.h"
#include "SceneLights.h"
#include "CollisionChecker.h"

// �V�[�������̃V�X�e���̃C���X�^���X�����N���X
class InSceneSystemManager : public Singleton_Base<InSceneSystemManager>
{
	friend class Singleton_Base<InSceneSystemManager>;

	InSceneSystemManager(){};
	~InSceneSystemManager();

	// �C���X�^���X
	// �V�[���I�u�W�F�N�g
	std::unique_ptr<SceneObjects> pSceneObjects;

	// �V�[�����C�e�B���O
	std::unique_ptr<SceneLights> pSceneLights;

	// �Փ˔���
	std::unique_ptr<CollisionChecker> pCollisionChecker;
public:
	void Init();

	SceneObjects& GetSceneObjects() { return *pSceneObjects; }
	SceneLights& GetSceneLights() { return *pSceneLights; }
	CollisionChecker& GetCollisonChecker() { return *pCollisionChecker; }
};


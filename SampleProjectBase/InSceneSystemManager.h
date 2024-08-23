#pragma once

// �V�[�����V�X�e���N���X
#include "SceneObjects.h"
#include "SceneLights.h"
#include "CollisionChecker.h"

// �J����
class CP_Camera;

// �V�[�������̃V�X�e���̃C���X�^���X�����N���X
class InSceneSystemManager : public Singleton_Base<InSceneSystemManager>
{
	friend class Singleton_Base<InSceneSystemManager>;

	/// @brief ���C���J����
	CP_Camera* pMainCamera;
	
	/// @brief �V�[���I�u�W�F�N�g
	std::unique_ptr<SceneObjects> pSceneObjects;

	/// @brief �V�[�����C�e�B���O
	std::unique_ptr<SceneLights> pSceneLights;

	/// @brief �Փ˔���
	std::unique_ptr<CollisionChecker> pCollisionChecker;

	InSceneSystemManager() : pMainCamera(nullptr) {}
	~InSceneSystemManager();

public:
	void Init();

	/// @brief  ���C���J�������擾
	/// @return 
	CP_Camera& GetMainCamera();

	/// @brief �V�[���̃I�u�W�F�N�g�z����擾
	/// @return �I�u�W�F�N�g�z��̎Q��
	SceneObjects& GetSceneObjects();

	/// @brief �V�[���̃��C�e�B���O�z����擾
	/// @return ���C�e�B���O�z��̎Q��
	SceneLights& GetSceneLights();

	/// @brief �V�[���̏Փ˔���V�X�e�����擾
	/// @return �Փ˔���V�X�e���̎Q��
	CollisionChecker& GetCollisonChecker() { return *pCollisionChecker; }

	// �J�������Z�b�g����
	void SetCamera(CP_Camera& _camera);
};


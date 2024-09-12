#pragma once

// �V�[�����V�X�e���N���X
#include "SceneObjects.h"
#include "SceneLights.h"
#include "CollisionChecker.h"

// �V�[�������̃V�X�e���̃C���X�^���X�����N���X
class InSceneSystemManager : public Singleton_Base<InSceneSystemManager>
{
	friend class Singleton_Base<InSceneSystemManager>;

	/// @brief �V�[������RigidBody
	std::list<CP_RigidBody*> pSceneRbs;

	/// @brief ���C���J����
	CP_Camera* pMainCamera;
	
	/// @brief �V�[���I�u�W�F�N�g
	std::unique_ptr<SceneObjects> pSceneObjects;

	/// @brief �V�[�����C�e�B���O
	std::unique_ptr<SceneLights> pSceneLights;

	InSceneSystemManager() : pMainCamera(nullptr) {}
	~InSceneSystemManager();

public:
	void Init();

	/// @brief �ϐ���������āA�V������������
	void Reset();

	/// @brief  ���C���J�������擾
	/// @return 
	CP_Camera& GetMainCamera();

	/// @brief �V�[���̃I�u�W�F�N�g�z����擾
	/// @return �I�u�W�F�N�g�z��̎Q��
	SceneObjects& GetSceneObjects();

	/// @brief �V�[���̃��C�e�B���O�z����擾
	/// @return ���C�e�B���O�z��̎Q��
	SceneLights& GetSceneLights();

	// �J�������Z�b�g����
	void SetCamera(CP_Camera& _camera);

	/// @brief RigidBody��ǉ����� 
	/// @param _setRb �ǉ�����RigidBody
	void AddRigidBody(CP_RigidBody& _setRb);

	/// @brief RigidBody�����O���� 
	/// @param _removetRb ���O����RigidBody
	void RemoveRigidBody(CP_RigidBody& _removetRb);

	/// @brief BtTransform��Dx�ɍX�V����
	void UpdateTransformBtToDx();
};


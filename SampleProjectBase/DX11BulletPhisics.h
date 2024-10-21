#pragma once

#include "BulletDebugDraw.h"
#include "BulletContactCallBack.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

class CP_RigidBody;

/// @brief DX11��BulletPhisics���g�p���A�ϐ��Ɗ֐����Ǘ�����N���X
class DX11BulletPhisics : public Singleton_Base<DX11BulletPhisics>
{
	friend class Singleton_Base<DX11BulletPhisics>;

	/// @brief �V�[������RigidBody�I�u�W�F�N�g(���̂̂�)
	std::list<CP_RigidBody*> sceneRigidbodys;

	/// @brief �V�[������GhostObject
	std::list<CP_RigidBody*> sceneGhostObjects;

	/// @brief �Փˌ��o�̓����ݒ�
	std::unique_ptr<btDefaultCollisionConfiguration> pCollisionConfiguration;

	/// @brief �X�̏Փ˃y�A�ɑ΂��ďՓˌ��o
	std::unique_ptr<btCollisionDispatcher> pDispatcher;

	/// @brief �����V�~�����[�V�������ŏՓˌ��o
	std::unique_ptr<btBroadphaseInterface> pBroadphase;

	/// @brief ���̗̂͊w�v�Z
	std::unique_ptr<btSequentialImpulseConstraintSolver> pSolver;

	/// @brief �����V�~�����[�V�����S�̂��Ǘ�����
	std::unique_ptr<btDiscreteDynamicsWorld> pDynamicsWorld; 

	/// @brief GhostObject�̃��b�v���m������������N���X
	std::unique_ptr<btGhostPairCallback> pGhostPairCallback;

	/// @brief �f�o�b�O�`��
	std::unique_ptr<BulletDebugDraw> pDebugDraw;

	/// @brief �R�[���o�b�N���Ăяo���N���X
	std::unique_ptr<BulletContactCallBack> pContactCallBack;

	/// @brief �ő�T�u�X�e�b�v��
	u_int maxSubSteps;

	/// @brief �d�͒l
	DirectX::SimpleMath::Vector3 gravityValue;
public:
	/// @brief Bullet������
	void Init();

	/// @brief �X�V����
	void Update();

	/// @brief �������Ă���I�u�W�F�N�g�̃R�[���o�b�N���Ăяo��
	void CollisionCallBack();

	/// @brief �`�揈��
	void Draw();

	/// @brief ������ԂɏՓ˃I�u�W�F�N�g��ǉ�����
	/// @param _rigidBodyCp �ǉ�����RigidBody�R���|�[�l���g
	/// @param _groupId �ǉ����铖���蔻��̃O���[�v
	void AddCollObj(CP_RigidBody& _rigidBodyCp, int _groupId);

	/// @brief ������ԂɏՓ˃I�u�W�F�N�g���폜����
	/// @param _rigidBodyCp �폜����RigidBody�R���|�[�l���g
	void RemoveCollObj(CP_RigidBody& _rigidBodyCp);

	/// @brief �V�[�����Փ˃I�u�W�F�N�g�̍��W��Dx�ɍX�V������(Bt��Dx)
	void UpdateTransformBtToDx();

	/// @brief �V�[�����Փ˃I�u�W�F�N�g�̍��W���X�V����(Dx��Bt)
	void UpdateTransformDxToBt();

	/// @brief �\�������邩�Z�b�g����
	/// @param _setBool �\�������邩�t���O
	void SetDisplay(bool _setBool);

	// �Փ˃I�u�W�F�N�g���`�F�b�N
	u_int GetCollObjCnt() const;

	/// @brief �\�������邩�擾
	/// @return �\�������邩�H 
	bool GetDisplay() const;

	// �d�͒l���擾����
	DirectX::SimpleMath::Vector3 GetGravityValue() const;
private:
	DX11BulletPhisics();
	~DX11BulletPhisics() {}

	/// @brief ���[���h��ԓ��ɏՓ˃I�u�W�F�N�g�����邩�m�F
	/// @param _checkCollObj �`�F�b�N����Փ˃I�u�W�F�N�g
	/// @return ���[���h��ԓ��ɂ��邩�H
	bool IsExistCollObjInWorld(btCollisionObject& _checkCollObj);

	/// @brief �ՓˏI��������ΏI���X�V�������s��
	void UpdateColExit();
};


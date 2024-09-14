#pragma once

#include "BulletDebugDraw.h"

/// @brief DX11��BulletPhisics���g�p���A�ϐ��Ɗ֐����Ǘ�����N���X
class DX11BulletPhisics : public Singleton_Base<DX11BulletPhisics>
{
	friend class Singleton_Base<DX11BulletPhisics>;

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

	/// @brief �f�o�b�O�`��
	std::unique_ptr<BulletDebugDraw> pDebugDraw;

	/// @brief �ő�T�u�X�e�b�v��
	u_int maxSubSteps;

	/// @brief �d�͒l
	DirectX::SimpleMath::Vector3 gravityValue;
public:
	/// @brief Bullet������
	void Init();

	/// @brief �X�V����
	void Update();

	/// @brief �`�揈��
	void Draw();

	/// @brief ������ԂɏՓ˃I�u�W�F�N�g��ǉ�����
	/// @param _collObj �ǉ�����Փ˃I�u�W�F�N�g
	/// @param _groupId �ǉ����铖���蔻��̃O���[�v
	void AddCollObj(btCollisionObject& _collObj, int _groupId);

	/// @brief ������ԂɏՓ˃I�u�W�F�N�g���폜����
	/// @param _collObj �폜����Փ˃I�u�W�F�N�g
	void RemoveCollObj(btCollisionObject& _collObj);

	// �Փ˃I�u�W�F�N�g���`�F�b�N
	u_int GetCollObjCnt() const;

	// �d�͒l���擾����
	DirectX::SimpleMath::Vector3 GetGravityValue() const;
private:
	DX11BulletPhisics();
	~DX11BulletPhisics() {}

	/// @brief ���[���h��ԓ��ɏՓ˃I�u�W�F�N�g�����邩�m�F
	/// @param _checkCollObj �`�F�b�N����Փ˃I�u�W�F�N�g
	/// @return ���[���h��ԓ��ɂ��邩�H
	bool IsExistCollObjInWorld(btCollisionObject& _checkCollObj);
};


#pragma once
#include "Component.h"
#include "DX11BulletPhisics.h"
#include "CollisionTypeJudge.h"

class CP_Collider;

class CP_RigidBody : public Component
{
	friend class CollisionTypeJudge;

	/// @brief �����蔻��`��^�C�v
	enum ShapeType
	{
		Box,
		Sphere,
		Num
	};

	/// @brief �Փ˃I�u�W�F�N�g�ƁA�R���W�����`��̃y�A
	struct CollPair
	{
		/// @brief �Փ˃I�u�W�F�N�g
		std::unique_ptr<btCollisionObject> pCollisionObject;
		/// @brief ���̑���
		std::unique_ptr<btDefaultMotionState> pMotionState;
		// �Փ˂̃^�C�~���O�𔻒f����֐�
		std::unique_ptr<CollisionTypeJudge> pColTypeJudge;
		// �Փˌ`��
		CP_Collider* pColliderComp{ nullptr };
		/// @brief �������[�����g
		btVector3 inertia;
	};

	/// @brief �Փ˃p�����[�^(nullptr�Ȃ画�肪�Ȃ�)
	std::unique_ptr<CollPair> collider;

	/// @brief ����
	float mass;

	/// @brief ���C�W��(0�Ŋ��S�Ɋ���)
	float friction;

	/// @brief ��Ɍv�Z������
	bool isAwaking;

	/// @brief ���蔲���邩�H
	bool isTrigger;

	/// @brief �L�l�}�e�B�b�N
	bool isKinematic;

	/// @brief ���̐�
	static constexpr short AXIS_CNT = 3;

	/// @brief �Œ肵������]���t���O
	std::array<bool, AXIS_CNT> isFreezeRotation;
public:
	CP_RigidBody();
	~CP_RigidBody() {}

	// �R���|�[�l���g���ʊ֐�
	void Init() override;
	void OnDestroy() override;
	void OnEnableTrue() override;
	void OnEnableFalse() override;

	/// @brief �Ռ���������
	/// @param _power �p���[
	void AddImpulse(const DirectX::SimpleMath::Vector3& _power);

	/// @brief �͂�������
	/// @param _power �p���[
	void AddForce(const DirectX::SimpleMath::Vector3& _power);

	/// @brief �R���C�_�[���Z�b�g����
	/// @param _setCollider �Z�b�g����R���C�_�[�R���|�[�l���g
	void SetColliderShape(CP_Collider& _setCollider);
	
	/// @brief �R���C�_�[���폜����
	/// @param _removeCollider 
	void RemoveColliderShape(CP_Collider& _removeCollider);

	/// @brief ���ʂ��Z�b�g����
	/// @param _mass ����
	void SetMass(float _mass);

	/// @brief ���ʂ��Z�b�g����
	/// @param _friction �Z�b�g���門�C�W��
	void SetFriction(float _friction);

	/// @brief �ÓI�I�u�W�F�N�g�ɑJ�ڂ��Ȃ��悤�ɂ��邩�Z�b�g����
	/// @param _isAwake �ÓI�I�u�W�F�N�g�ɑJ�ڂ��Ȃ��悤�ɂ��邩�H
	void SetIsAwake(bool _isAwake);

	/// @brief ���Ԃ������Ȃ��悤�ɁighostObject�j�ɂ��邩�Z�b�g����
	/// @param _isTrigger  ���Ԃ������Ȃ��悤�ɂ��邩�H
	void SetIsTrigger(bool _isTrigger);

	/// @brief �L�l�}�e�B�b�N���Z�b�g
	/// @param _isKinematic �L�l�}�e�B�b�N
	void SetIsKinematic(bool _isKinematic);

	/// @brief DX����Bullet�Ƀg�����X�t�H�[����������
	void SetToBtTransform();

	/// @brief Bullet����DX�Ƀg�����X�t�H�[����������
	void SetToDXTransform();

	/// @brief ���x���Z�b�g
	/// @param _velocity ���x
	void SetVelocity(const DirectX::SimpleMath::Vector3& _velocity);

	/// @brief ���x���N���A����
	void ClearVelocity();

	/// @brief �Փ˃I�u�W�F�N�g���擾����
	/// @param _outObject ���ʃI�u�W�F�N�g
	btCollisionObject& GetCollisionObject();

	/// @brief �Փ˃^�C�v������擾
	/// @return �Փ˃^�C�v����N���X�ϐ�
	CollisionTypeJudge& GetColTypeJudge() const;

	/// @brief ���̂̑��x���擾����
	/// @return ���x
	DirectX::SimpleMath::Vector3 GetVelocity() const;

	void ImGuiDebug() override;

	/// @brief �Z�[�u����
	/// @param _data �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����V�[���f�[�^ 
	void Load(const nlohmann::json& _data) override;
private:
	void UpdateFreezeRotation();

	/// @brief �Q�[���I�u�W�F�N�g����R���C�_�[��T���ăZ�b�g����
	void FindSetCollider();

	/// @brief ���̂��쐬����
	void CreateRigidBody();

	/// @brief GohstObject���쐬����
	void CreateGhost();

	/// @brief ���̂ł�Ghost�ł����ʂɋN���鏈��
	void CommonCreateColObj();

	/// @brief btRigidBody�^�ɕϊ�����
	/// @return btRigidBpdy�Q��
	btRigidBody& CastRigidBody();

	/// @brief Dx����Bullet�̃g�����X�t�H�[�����擾����
	/// @param _outBtTransform Dx����Bullet��Transform���擾
	void CastDxToBtTransform(btTransform& _outBtTransform);

	/// @brief �ēx�R���C�_�[����蒼���iRigidBody����GhpstObject�ɂ������Ƃ��Ƃ��j
	void ReCreateCollider();

	/// @brief �Փ˒���Rb���X�g�ɑ��������ǉ�����
	/// @param _addRb �ǉ�����Rb
	void AddCurrentCollision(CP_RigidBody& _addRb);

	/// @brief Bullet��Transform���擾����
	/// @param _btTrans ����
	void GetBtTransform(btTransform& _btTrans);

	// ImGui�ŉ�]�Œ�Ɋւ���ҏW
	void ImGuiFreezeRot();
};


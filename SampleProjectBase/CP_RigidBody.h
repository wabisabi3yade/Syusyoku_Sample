#pragma once
#include "CloneComponent.h"

#include "DX11BulletPhisics.h"

/// @brief �d���⍄�̂��Ȃǂ̃p�����[�^�ێ��R���|�[�l���g
class CP_RigidBody : public Component, public CloneComponent<CP_RigidBody>
{
	/// @brief �Փ˃I�u�W�F�N�g(RigidBody, Ghost)
	std::unique_ptr<btCollisionObject> pCollisionObject;

	/// @brief ���̑���
	std::unique_ptr<btDefaultMotionState> pMotionState;

	/// @brief �����蔻��̌`��
	btCollisionShape* pShape;

	/// @brief �������[�����g
	DirectX::SimpleMath::Vector3 inertia;

	/// @brief ����(0.0�Ȃ�ÓI�I�u�W�F�N�g)
	float mass;

	/// @brief ���̂��H
	bool isRigidbody;

	/// @brief �d�͂��󂯂邩�H
	bool isGravity;

	/// @brief �����v�Z���s�����H
	bool isKinematic;

	/// @brief �`�󂪃Z�b�g����Ă��邩�H
	bool isSetShape;
public:
	CP_RigidBody();
	CP_RigidBody(const CP_RigidBody& _other);

	CP_RigidBody& operator=(const CP_RigidBody& _other);

	void Init() override;
	void Update() override;
	void ImGuiSetting() override;
	void OnDestroy() override;

	/// @brief ���ʂ��Z�b�g���A�Čv�Z
	/// @param _mass ����
	void SetMass(float _mass);

	/// @brief �����蔻��̌`����Z�b�g
	/// @param _shape �`��
	void SetShape(btCollisionShape& _shape);

	/// @brief �`����폜����
	void RemoveShape();

	/// @brief Dx����Bt�̃g�����X�t�H�[���ɍ��킹��
	/// @param _dxTransform dx�̃g�����X�t�H�[��
	void SetTransformDxToBt();

	/// @brief  Bt����Dx�̃g�����X�t�H�[���ɍ��킹��
	void SetTransformBtToDx();
	
	/// @brief �Z�[�u����
	/// @param _data �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����V�[���f�[�^ 
	void Load(const nlohmann::json& _data) override;
private:
	void Copy(const CP_RigidBody& _other);

	void Start() override;

	void OnEnableTrue() override;
	void OnEnableFalse() override;

	/// @brief Bullet��Transform�ɕϊ�����
	/// @param _btTransform �i�[����Bullet Transform
	void ToBtTransform(btTransform& _btTransform);

	/// @brief ���̂��ǂ��Z�b�g
	/// @param _isRB ���̂��H
	void SetIsRigidbody(bool _isRB);

	/// @brief �d�͂��󂯂邩�Z�b�g
	/// @param _isGravity �d�͂��󂯂邩�H
	void SetIsGravity(bool _isGravity);

	/// @brief �Փ˃I�u�W�F�N�g��������A������Ԃ���O��
	void RemoveCollObject();

	/// @brief �Փ˃I�u�W�F�N�g�쐬
	void CreateCollObject();

	/// @brief ���̂��쐬
	void CreateRB();

	/// @brief Ghost���쐬
	void CreateGhost();

	/// @brief �`��ƍ��̂̍��W�����킹��
	void CollisionSetTransform();

	/// @brief �Փ˃I�u�W�F�N�g��btRigidBody�ɕϊ�
	/// @return cast�ϊ������|�C���^
	btRigidBody* CastRigidBody();

	/// @brief �����V�~�����[�V�����ɏՓ˃I�u�W�F�N�g��ǉ�����
	void AddCollisionToWorld();
};


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

	/// @brief �����蔻��̃I�t�Z�b�g���W
	DirectX::SimpleMath::Vector3 collOffsetPos;

	/// @brief �����蔻��̃I�t�Z�b�g�p�x
	DirectX::SimpleMath::Vector3 collOffsetAngle;

	/// @brief ����(0.0�Ȃ�ÓI�I�u�W�F�N�g)
	float mass;

	/// @brief ���̂��H
	bool isRigidBody;

	/// @brief �`�󂪃Z�b�g����Ă��邩�H
	bool isSetShape;
public:
	CP_RigidBody();
	CP_RigidBody(const CP_RigidBody& _other);
	~CP_RigidBody();

	CP_RigidBody& operator=(const CP_RigidBody& _other);

	void Awake() override;

	void LateUpdate() override;

	void ImGuiSetting() override;

	/// @brief ���ʂ��Z�b�g���A�Čv�Z
	/// @param _mass ����
	void SetMass(float _mass);

	/// @brief �����蔻��̌`����Z�b�g
	/// @param _shape �`��
	void SetShape(btCollisionShape& _shape);

	/// @brief �I�u�W�F�N�g����̃I�t�Z�b�g���W���Z�b�g
	/// @param _originPos �I�t�Z�b�g���W
	void SetCollOffsetPos(const DirectX::SimpleMath::Vector3& _offsetPos);

	/// @brief �I�u�W�F�N�g����̃I�t�Z�b�g�p�x���Z�b�g
	/// @param _offsetAngles �I�t�Z�b�g�p�x
	void SetCollAngles(const DirectX::SimpleMath::Vector3& _offsetAngles);

	/// @brief �`����폜����
	void RemoveShape();
private:
	void Copy(const CP_RigidBody& _other);

	void Start() override;

	/// @brief Bullet��Transform�ɕϊ�����
	/// @param _btTransform �i�[����Bullet Transform
	void ToBtTransform(btTransform& _btTransform);

	/// @brief ���̂��ǂ��Z�b�g
	/// @param _isRB ���̂��H
	void SetRigidBody(bool _isRB);

	/// @brief �Փ˃I�u�W�F�N�g��������A������Ԃ���O��
	void RemoveCollObject();

	/// @brief �Փ˃I�u�W�F�N�g�쐬
	void CreateCollObject();

	/// @brief ���̂��쐬
	void CreateRB();

	/// @brief Ghost���쐬
	void CreateGhost();

	/// @brief Transform���X�V����
	void UpdateBtTransform();
};


#pragma once
#include "Component.h"
#include "DX11BulletPhisics.h"

class CP_Collider;

class CP_RigidBody2 : public Component
{

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
		std::unique_ptr<btRigidBody> pCollisionObject;
		/// @brief ���̑���
		std::unique_ptr<btDefaultMotionState> pMotionState;
		/// @brief �R���W�����`��
		/*std::unique_ptr<btCollisionShape> pColliderShape;*/
		CP_Collider* pCollider{ nullptr };
		/// @brief �������[�����g
		DirectX::SimpleMath::Vector3 inertia;
	};
	//std::list<std::unique_ptr<CollPair>> colliders;	// �Փˌ`�󃊃X�g
	std::unique_ptr<CollPair> collider;

	/// @brief ����
	float mass;

	/// @brief ��Ɍv�Z������
	bool isAwake;

	bool isTrigger;
public:
	CP_RigidBody2();
	~CP_RigidBody2() {}

	void Init() override;

	void Start() override;

	void OnDestroy() override;

	void OnChangeTransform() override;

	void OnEnableTrue() override;
	void OnEnableFalse() override;

	void SetColliderShape(CP_Collider& _setCollider);
	void SetColliderShape(ShapeType _shapeType);

	void RemoveColliderShape(CP_Collider& _removeCollider);

	void SetMass(float _mass);
	void SetIsAwake(bool _isAwake);

	void SetToBtTransform();
	void SetToDXTransform();

	void ImGuiSetting() override;

	/// @brief �Z�[�u����
	/// @param _data �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����V�[���f�[�^ 
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief Bullet��Transform���擾����
	/// @param _btTrans ����
	void GetBtTransform(btTransform& _btTrans);

	/// @brief �Q�[���I�u�W�F�N�g����R���C�_�[��T���ăZ�b�g����
	void FindSetCollider();
};


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
		std::unique_ptr<btCollisionObject> pCollisionObject;
		/// @brief ���̑���
		std::unique_ptr<btDefaultMotionState> pMotionState;
		/// @brief �R���W�����`��
		/*std::unique_ptr<btCollisionShape> pColliderShape;*/
		CP_Collider* pColliderComp{ nullptr };
		/// @brief �������[�����g
		DirectX::SimpleMath::Vector3 inertia;
	};
	//std::list<std::unique_ptr<CollPair>> colliders;	// �Փˌ`�󃊃X�g
	std::unique_ptr<CollPair> collider;

	/// @brief ����
	float mass;

	/// @brief ��Ɍv�Z������
	bool isAwake;

	/// @brief ���蔲���邩�H
	bool isTrigger;
public:
	CP_RigidBody2();
	~CP_RigidBody2() {}

	// �R���|�[�l���g���ʊ֐�
	void Init() override;
	void Start() override;
	void OnDestroy() override;
	void OnChangeTransform() override;
	void OnEnableTrue() override;
	void OnEnableFalse() override;

	/// @brief �R���C�_�[���Z�b�g����
	/// @param _setCollider �Z�b�g����R���C�_�[�R���|�[�l���g
	void SetColliderShape(CP_Collider& _setCollider);
	
	/// @brief �R���C�_�[���폜����
	/// @param _removeCollider 
	void RemoveColliderShape(CP_Collider& _removeCollider);

	/// @brief ���ʂ��Z�b�g����
	/// @param _mass 
	void SetMass(float _mass);

	/// @brief �ÓI�I�u�W�F�N�g�ɑJ�ڂ��Ȃ��悤�ɂ��邩�Z�b�g����
	/// @param _isAwake �ÓI�I�u�W�F�N�g�ɑJ�ڂ��Ȃ��悤�ɂ��邩�H
	void SetIsAwake(bool _isAwake);

	/// @brief ���Ԃ������Ȃ��悤�ɁighostObject�j�ɂ��邩�Z�b�g����
	/// @param _isTrigger  ���Ԃ������Ȃ��悤�ɂ��邩�H
	void SetIsTrigger(bool _isTrigger);

	/// @brief DX����Bullet�Ƀg�����X�t�H�[����������
	void SetToBtTransform();

	/// @brief Bullet����DX�Ƀg�����X�t�H�[����������
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

	/// @brief btRigidBody�^�ɕϊ�����
	/// @return btRigidBpdy�Q��
	btRigidBody& CastRigidBody();

	/// @brief �ēx�R���C�_�[����蒼���iRigidBody����GhpstObject�ɂ������Ƃ��Ƃ��j
	void ReCreateCollider();
};


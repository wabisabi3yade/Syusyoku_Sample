#include "pch.h"
#include "CP_RigidBody2.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "CP_Collider.h"
#include "InSceneSystemManager.h"

using namespace HashiTaku;
namespace DX = DirectX::SimpleMath;

constexpr float SLEEP_LINEAR(0.01f);
constexpr float SLEEP_ANGLE(0.01f);

CP_RigidBody2::CP_RigidBody2()
	: mass(1.0f), isAwake(false), isTrigger(false)
{

}

void CP_RigidBody2::Init()
{
	FindSetCollider();
}

void CP_RigidBody2::Start()
{

}

void CP_RigidBody2::OnDestroy()
{
	DX11BulletPhisics* pEngine = DX11BulletPhisics::GetInstance();
	InSceneSystemManager::GetInstance()->RemoveRigidBody(*this);
}

void CP_RigidBody2::OnChangeTransform()
{
	SetToBtTransform();
}

void CP_RigidBody2::OnEnableTrue()
{
	// ���[���h�ɒǉ�
	if (!collider) return;
	DX11BulletPhisics::GetInstance()->AddCollObj(*collider->pCollisionObject, 0);
	InSceneSystemManager::GetInstance()->AddRigidBody(*this);
}

void CP_RigidBody2::OnEnableFalse()
{
	if (!collider) return;
	DX11BulletPhisics::GetInstance()->RemoveCollObj(*collider->pCollisionObject);
	InSceneSystemManager::GetInstance()->RemoveRigidBody(*this);
}

void CP_RigidBody2::SetColliderShape(CP_Collider& _setCollider)
{
	if (collider) return;

	//�ǉ�����
	collider = std::make_unique<CollPair>();
	collider->pColliderComp = &_setCollider;

	// DX���ɍ��킹��
	Transform& transform = GetTransform();
	btVector3 origin = Bullet::ToBtVector3(transform.GetPosition());
	btQuaternion rot = Bullet::ToBtQuaeternion(transform.GetRotation());
	btTransform	bulletTrans;
	bulletTrans.setIdentity();
	bulletTrans.setOrigin(origin);
	bulletTrans.setRotation(rot);

	//if (isTrigger)	// ghost�Ȃ�
	//{
	//	collider->pCollisionObject = std::make_unique<btGhostObject>();
	//	collider->pCollisionObject->setWorldTransform(bulletTrans);
	//	collider->pCollisionObject->setCollisionShape(&collider->pColliderComp->GetColliderShape());
	//	DX11BulletPhisics::GetInstance()->AddCollObj(*collider->pCollisionObject, 0);
	//	InSceneSystemManager::GetInstance()->AddRigidBody(*this);
	//	return;
	//}

	bool isDynamic = mass > Mathf::epsilon;

	// �������v�Z����
	collider->inertia = DX::Vector3::Zero;
	if (isDynamic)
	{
		btVector3 btInertia;
		collider->inertia = Bullet::ToDXVector3(btInertia);
	}
	collider->pMotionState = std::make_unique<btDefaultMotionState>(bulletTrans);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(
		Bullet::ToBtScalar(mass),
		collider->pMotionState.get(),
		&collider->pColliderComp->GetColliderShape(),
		Bullet::ToBtVector3(collider->inertia));
	collider->pCollisionObject = std::make_unique<btRigidBody>(rbInfo);

	if (isAwake)	// ��Ɍv�Z����Ȃ�X���[�u��Ԃɂ��Ȃ�
		collider->pCollisionObject->setActivationState(DISABLE_DEACTIVATION);

	// ���[���h�ɒǉ�
	DX11BulletPhisics::GetInstance()->AddCollObj(*collider->pCollisionObject, 0);
	InSceneSystemManager::GetInstance()->AddRigidBody(*this);
}

void CP_RigidBody2::RemoveColliderShape(CP_Collider& _removeCollider)
{
	if (!collider) return;
	if (collider->pColliderComp != &_removeCollider) return;

	DX11BulletPhisics::GetInstance()->RemoveCollObj(*collider->pCollisionObject);
	collider.reset();
}

void CP_RigidBody2::SetMass(float _mass)
{
	mass = _mass;

	if (!collider || isTrigger) return;

	bool isDynamic = mass >= Mathf::epsilon;

	btTransform bulletTrans;
	bulletTrans.setIdentity();

	btCollisionShape& shape = *collider->pCollisionObject->getCollisionShape();

	collider->inertia = DX::Vector3::Zero;
	if (mass != 0.0f) {
		btVector3 localInertia;
		shape.calculateLocalInertia(mass, localInertia);  // �������[�����g���v�Z
		collider->inertia = Bullet::ToDXVector3(localInertia);
	}

	btRigidBody& rigid = CastRigidBody();

	rigid.setMassProps(mass, Bullet::ToBtVector3(collider->inertia));
	rigid.updateInertiaTensor();
}

void CP_RigidBody2::SetIsAwake(bool _isAwake)
{
	isAwake = _isAwake;

	if (!collider) return;

	// Awake��Ԃɍ��̂����킹��
	if (isAwake)
		collider->pCollisionObject->setActivationState(DISABLE_DEACTIVATION);
	else
		collider->pCollisionObject->setActivationState(ACTIVE_TAG);
}

void CP_RigidBody2::SetIsTrigger(bool _isTrigger)
{
	isTrigger = _isTrigger;

	//ReCreateCollider();
}

void CP_RigidBody2::SetToBtTransform()
{
	assert(collider && "�R���C�_�[������܂���");

	Transform& transform = GetTransform();
	btVector3 origin = Bullet::ToBtVector3(transform.GetPosition());
	btQuaternion rot = Bullet::ToBtQuaeternion(transform.GetRotation());

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(origin);
	trans.setRotation(rot);

	//if (isTrigger)	// ghost�I�u�W�F�N�g�Ȃ�
	//{
	//	collider->pCollisionObject->setWorldTransform(trans);
	//}
	//else	// rigidBody�Ȃ�
	//{
		btRigidBody& rigid = CastRigidBody();
		collider->pMotionState->setWorldTransform(trans);
		rigid.setMotionState(collider->pMotionState.get());
	//}
}

void CP_RigidBody2::SetToDXTransform()
{
	btTransform bulletTransform;
	GetBtTransform(bulletTransform);

	Transform& dxTransform = GetTransform();
	dxTransform.SetPosition(Bullet::ToDXVector3(bulletTransform.getOrigin()));
	dxTransform.SetRotation(Bullet::ToDXQuaternion(bulletTransform.getRotation()));
}

void CP_RigidBody2::ImGuiSetting()
{
	if (ImGui::Button("SetShape"))
		FindSetCollider();

	// ����
	float imMass = mass;
	if (ImGui::DragFloat("mass", &imMass, 0.01f, 0.0f, 1000.0f))
		SetMass(imMass);

	// �ÓI�I�u�W�F�N�g�ɂ��Ȃ�
	bool isImBool = isAwake;
	if (ImGui::Checkbox("IsAwake", &isImBool))
		SetIsAwake(isImBool);

	isImBool = isTrigger;
	if (ImGui::Checkbox("IsTrigger", &isImBool))
		SetIsTrigger(isImBool);
}

nlohmann::json CP_RigidBody2::Save()
{
	auto data = Component::Save();
	data["mass"] = mass;
	data["isAwake"] = isAwake;
	data["isTrigger"] = isTrigger;
	return data;
}

void CP_RigidBody2::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	LoadJsonFloat("mass", mass, _data);
	LoadJsonBoolean("isAwake", isAwake, _data);
	LoadJsonBoolean("isTrigger", isTrigger, _data);

	ReCreateCollider();
}

void CP_RigidBody2::GetBtTransform(btTransform& _btTrans)
{
	if (!collider) return;

	if (isTrigger)
		_btTrans = collider->pCollisionObject->getWorldTransform();
	else
		collider->pMotionState->getWorldTransform(_btTrans);
}

void CP_RigidBody2::FindSetCollider()
{
	CP_Collider* pCol = GetGameObject().GetComponent<CP_Collider>();
	if (!pCol) return;

	SetColliderShape(*pCol);
}

btRigidBody& CP_RigidBody2::CastRigidBody()
{
	assert(collider && "�R���C�_�[���쐬����Ă��܂���");
	assert(!isTrigger && "RigidBody�ł͂���܂���");

	return static_cast<btRigidBody&>(*collider->pCollisionObject);
}

void CP_RigidBody2::ReCreateCollider()
{
	if (!collider) return;

	CP_Collider& colliderComp = *collider->pColliderComp;

	// ���ɂ���Ȃ烏�[���h����폜����
	RemoveColliderShape(colliderComp);

	// ���݂̃p�����[�^�ō�蒼��
	SetColliderShape(colliderComp);
}

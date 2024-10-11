#include "pch.h"
#include "CP_RigidBody.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "DX11BulletPhisics.h"
#include "InSceneSystemManager.h"
#include "GameObject.h"

namespace DX = DirectX::SimpleMath;
using namespace HashiTaku;

constexpr float DEFAULT_MERGIN(0.04f);	// �f�t�H���g�̃R���W�����`��}�[�W��

CP_RigidBody::CP_RigidBody()
	: pShape(nullptr), mass(1.0f), isTrigger(false), isGravity(true), isSetShape(false), isAlreadySendCol(false)
{
}

CP_RigidBody::CP_RigidBody(const CP_RigidBody& _other)
	: pShape(nullptr), isSetShape(false)
{
	Copy(_other);
}

CP_RigidBody& CP_RigidBody::operator=(const CP_RigidBody& _other)
{
	Copy(_other);

	return *this;
}

void CP_RigidBody::Init()
{
	// �V�[������RigidBody�Ǘ��ɒǉ�
	//InSceneSystemManager::GetInstance()->AddRigidBody(*this);
}

void CP_RigidBody::Update()
{
}

void CP_RigidBody::ImGuiSetting()
{
	if (!isSetShape) return;

	btTransform bullet;
	GetBulletTransform(bullet);
	DX::Vector3 pos = Bullet::ToDXVector3(bullet.getOrigin());
	DX::Quaternion quaternion = Bullet::ToDXQuaternion(bullet.getRotation());
	ImGuiMethod::Text(pos);
	ImGuiMethod::Text(Quat::ToEulerAngles(quaternion));

	float changeMass = mass;
	if (ImGui::DragFloat("mass", &changeMass, 0.1f, 0.0f, 500.0f))
		SetMass(changeMass);

	bool isChange = isGravity;
	if (ImGui::Checkbox("gravity", &isChange))
		SetIsGravity(isChange);

	isChange = isTrigger;
	if (ImGui::Checkbox("trigger", &isChange))
		SetIsTrigger(isChange);
}

void CP_RigidBody::OnDestroy()
{
	RemoveCollObject();

	//InSceneSystemManager::GetInstance()->RemoveRigidBody(*this);
}

void CP_RigidBody::OnChangeTransform()
{
	SetTransformDxToBt();
}

void CP_RigidBody::Start()
{
	SetMass(mass);
	SetIsGravity(isGravity);
}

void CP_RigidBody::OnEnableTrue()
{
	// ���[���h�ɒǉ�
	AddCollisionToWorld();
	//InSceneSystemManager::GetInstance()->AddRigidBody(*this);
}

void CP_RigidBody::OnEnableFalse()
{
	// ���[���h����폜
	if (pCollisionObject)
		DX11BulletPhisics::GetInstance()->RemoveCollObj(*pCollisionObject);

	//InSceneSystemManager::GetInstance()->RemoveRigidBody(*this);
}

void CP_RigidBody::ToBtTransform(btTransform& _btTransform)
{
	const Transform& dxTransform = GetTransform();
	_btTransform.setIdentity();
	_btTransform.setRotation(Bullet::ToBtQuaeternion(dxTransform.GetRotation()));
	_btTransform.setOrigin(Bullet::ToBtVector3(dxTransform.GetPosition()));
}

void CP_RigidBody::SetMass(float _mass)
{
	mass = std::max(_mass, 0.0f);

	/*if (!isSetShape) return;*/

	// �ړ�����I�u�W�F�N�g���ǂ���
	bool isDynamic = mass > Mathf::epsilon;

	// �������[�����g�Čv�Z
	if (isDynamic)
	{
		btVector3 btInertia;
		pShape->calculateLocalInertia(mass, btInertia);
		inertia = Bullet::ToDXVector3(btInertia);
	}
	else
		inertia = DX::Vector3::Zero;

	if (!isTrigger)	// ���̂�������
	{
		btRigidBody* pRigidBody = btRigidBody::upcast(pCollisionObject.get());

		pRigidBody->setMassProps(mass, Bullet::ToBtVector3(inertia));
		pRigidBody->updateInertiaTensor();
	}
}

void CP_RigidBody::SetShape(btCollisionShape& _shape)
{
	isSetShape = true;
	pShape = &_shape;
	pShape->setMargin(DEFAULT_MERGIN);
	CreateCollObject();
}

void CP_RigidBody::RemoveShape()
{
	isSetShape = false;
	pShape = nullptr;
	RemoveCollObject();
}

void CP_RigidBody::SetTransformDxToBt()
{
	if (!isSetShape) return;

	btTransform bulletTransform;
	bulletTransform.setIdentity();
	ToBtTransform(bulletTransform);

	SetBulletTransform(bulletTransform);
}

void CP_RigidBody::SetTransformBtToDx()
{
	if (!isSetShape) return;

	btTransform bulletTransform;
	GetBulletTransform(bulletTransform);

	Transform& dxTransform = GetTransform();
	dxTransform.SetPosition(Bullet::ToDXVector3(bulletTransform.getOrigin()));
	dxTransform.SetRotation(Bullet::ToDXQuaternion(bulletTransform.getRotation()));
}

nlohmann::json CP_RigidBody::Save()
{
	auto data = Component::Save();
	data["mass"] = mass;
	data["isRigid"] = isTrigger;
	data["isGravity"] = isGravity;
	return data;
}

void CP_RigidBody::Load(const nlohmann::json& _data)
{
	Component::Load(_data);
	LoadJsonFloat("mass", mass, _data);
	LoadJsonBoolean("isRigid", isTrigger, _data);
	LoadJsonBoolean("isGravity", isGravity, _data);
}

void CP_RigidBody::Copy(const CP_RigidBody& _other)
{
	if (this == &_other) return;

	Component::operator=(_other);
	mass = _other.mass;
	isTrigger = _other.isTrigger;
	isGravity = _other.isGravity;
}

void CP_RigidBody::SetIsTrigger(bool _isTrigger)
{
	// �ύX�O�Ɠ����Ȃ�
	if (isTrigger == _isTrigger) return;

	isTrigger = _isTrigger;
	CreateCollObject();	// �����蔻����쐬���Ȃ���
}

void CP_RigidBody::SetIsGravity(bool _isGravity)
{
	if (!isSetShape) return;
	isGravity = _isGravity;

	if (isTrigger) return;
	btRigidBody* btRb = static_cast<btRigidBody*>(pCollisionObject.get());
	btRb->activate();

	/*DX::Vector3 gravityVal = DX11BulletPhisics::GetInstance()->GetGravityValue();
	btRb->setGravity(Bullet::ToBtVector3(gravityVal));*/
/*else
{
	btRb->setGravity(btVector3(0, 0, 0));
	btRb->setLinearVelocity(btVector3(0, 0, 0));
	btRb->setAngularVelocity(btVector3(0, 0, 0));
}*/
}

void CP_RigidBody::RemoveCollObject()
{
	// ���[���h����폜
	if (!pCollisionObject || !isAlreadySendCol) return;

	isAlreadySendCol = false;
	DX11BulletPhisics::GetInstance()->RemoveCollObj(*pCollisionObject);
	pCollisionObject.reset();
}

void CP_RigidBody::CreateCollObject()
{
	if (!isSetShape)
	{
		HASHI_DEBUG_LOG("��Ɍ`���ݒ肵�Ă�������");
		return;
	}
	if (pCollisionObject)
	{
		// ���[���h����폜
		RemoveCollObject();
	}

	if (!isTrigger)	// ���̂�
	{
		CreateRB();

	}
	else // Ghost��
	{
		CreateGhost();
	}

	// ���[���h�ɓ����蔻���ǉ�
	AddCollisionToWorld();
}

void CP_RigidBody::CreateRB()
{
	btTransform bulletTrans;
	ToBtTransform(bulletTrans);
	pMotionState = std::make_unique<btDefaultMotionState>(bulletTrans);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(
		Bullet::ToBtScalar(mass),
		pMotionState.get(),
		pShape,
		Bullet::ToBtVector3(inertia)
	);

	pCollisionObject = std::make_unique<btRigidBody>(rbInfo);
	pCollisionObject->setWorldTransform(bulletTrans);

	// �����x��0�ɂ���
	btRigidBody* btRb = btRigidBody::upcast(pCollisionObject.get());
	btRb->setLinearVelocity(btVector3(0, 0, 0));
	btRb->setAngularVelocity(btVector3(0, 0, 0));
}

void CP_RigidBody::CreateGhost()
{
	pCollisionObject = std::make_unique<btGhostObject>();
	pCollisionObject->setCollisionShape(pShape);

	btTransform bulletTrans;
	ToBtTransform(bulletTrans);
	SetBulletTransform(bulletTrans);
}

btRigidBody* CP_RigidBody::CastRigidBody()
{
	if (isTrigger || !isSetShape) return nullptr;

	return static_cast<btRigidBody*>(pCollisionObject.get());
}

void CP_RigidBody::AddCollisionToWorld()
{
	if (isAlreadySendCol) return;

	isAlreadySendCol = true;

	// �Q�[���I�u�W�F�N�g�̃��C���[��ID�Ƃ���
	int groupId = static_cast<int>(gameObject->GetLayer().GetType());

	if (GetIsActive() && pCollisionObject)
		DX11BulletPhisics::GetInstance()->AddCollObj(*pCollisionObject, groupId);
}

void CP_RigidBody::SetBulletTransform(const btTransform& _set)
{
	pCollisionObject->setWorldTransform(_set);

	if (!isTrigger)
	{
		pMotionState->setWorldTransform(_set);
	}
}

void CP_RigidBody::GetBulletTransform(btTransform& _get)
{
	_get = pCollisionObject->getWorldTransform();

	if (!isTrigger)	// ���̂Ȃ�
		pMotionState->getWorldTransform(_get);
}

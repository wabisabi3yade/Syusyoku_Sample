#include "pch.h"
#include "CP_RigidBody2.h"
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
	// ワールドに追加
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

	//追加する
	collider = std::make_unique<CollPair>();
	collider->pCollider = &_setCollider;

	bool isDynamic = mass > Mathf::epsilon;

	// DX側に合わせる
	Transform& transform = GetTransform();
	btVector3 origin = Bullet::ToBtVector3(transform.GetPosition());
	btQuaternion rot = Bullet::ToBtQuaeternion(transform.GetRotation());
	btTransform	bulletTrans;
	bulletTrans.setIdentity();
	bulletTrans.setOrigin(origin);
	bulletTrans.setRotation(rot);

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
		&collider->pCollider->GetColliderShape(),
		Bullet::ToBtVector3(collider->inertia));
	collider->pCollisionObject = std::make_unique<btRigidBody>(rbInfo);

	// ワールドに追加
	DX11BulletPhisics::GetInstance()->AddCollObj(*collider->pCollisionObject, 0);
	InSceneSystemManager::GetInstance()->AddRigidBody(*this);
}

void CP_RigidBody2::SetColliderShape(ShapeType _shapeType)
{
	//	if (collider)
	//	{
	//		DX11BulletPhisics::GetInstance()->RemoveCollObj(*collider->pCollisionObject);
	//	}
	//
	//	//追加する
	//	collider = std::make_unique<CollPair>();
	//
	//	switch (_shapeType)
	//	{
	//	case CP_RigidBody2::Box:
	//		collider->pColliderShape = std::make_unique<btBoxShape>(btVector3(1.0f, 1.0f, 1.0f));
	//		break;
	//	case CP_RigidBody2::Sphere:
	//		break;
	//	default:
	//		break;
	//	}
	//
	//	bool isDynamic = mass > Mathf::epsilon;
	//
	//	btTransform bulletTrans;
	//	bulletTrans.setIdentity();
	//
	//	collider->inertia = DX::Vector3::Zero;
	//	if (isDynamic)
	//	{
	//		btVector3 btInertia;
	//		collider->inertia = Bullet::ToDXVector3(btInertia);
	//	}
	//	collider->pMotionState = std::make_unique<btDefaultMotionState>(bulletTrans);
	//
	//	btRigidBody::btRigidBodyConstructionInfo rbInfo(
	//		Bullet::ToBtScalar(mass),
	//		collider->pMotionState.get(),
	//		collider->pColliderShape.get(),
	//		Bullet::ToBtVector3(collider->inertia));
	//	collider->pCollisionObject = std::make_unique<btRigidBody>(rbInfo);
	//
	//	// ワールドに追加
	//	DX11BulletPhisics::GetInstance()->AddCollObj(*collider->pCollisionObject, 0);
	//
	//	SetToBtTransform();
}

void CP_RigidBody2::RemoveColliderShape(CP_Collider& _removeCollider)
{
	if (!collider) return;
	if (collider->pCollider != &_removeCollider) return;

	DX11BulletPhisics::GetInstance()->RemoveCollObj(*collider->pCollisionObject);
	collider.reset();
}

void CP_RigidBody2::SetMass(float _mass)
{
	mass = _mass;

	if (!collider) return;

	bool isDynamic = mass >= Mathf::epsilon;

	btTransform bulletTrans;
	bulletTrans.setIdentity();

	btCollisionShape& shape = *collider->pCollisionObject->getCollisionShape();

	collider->inertia = DX::Vector3::Zero;
	if (mass != 0.0f) {
		btVector3 localInertia;
		shape.calculateLocalInertia(mass, localInertia);  // 慣性モーメントを計算
		collider->inertia = Bullet::ToDXVector3(localInertia);
	}

	collider->pCollisionObject->setMassProps(mass, Bullet::ToBtVector3(collider->inertia));
	collider->pCollisionObject->updateInertiaTensor();
}

void CP_RigidBody2::SetIsAwake(bool _isAwake)
{
	isAwake = _isAwake;
}

void CP_RigidBody2::SetToBtTransform()
{
	assert(collider && "コライダーがありません");

	Transform& transform = GetTransform();
	btVector3 origin = Bullet::ToBtVector3(transform.GetPosition());
	btQuaternion rot = Bullet::ToBtQuaeternion(transform.GetRotation());

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(origin);
	trans.setRotation(rot);

	collider->pMotionState->setWorldTransform(trans);
	collider->pCollisionObject->setMotionState(collider->pMotionState.get());
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


	float imMass = mass;
	if (ImGui::DragFloat("mass", &imMass, 0.01f, 0.0f, 1000.0f))
	{
		SetMass(imMass);
	}

	bool isImAwake = isAwake;
	if (ImGui::Checkbox("IsAwake", &isAwake))
	{

	}
}

nlohmann::json CP_RigidBody2::Save()
{
	auto data = Component::Save();
	data["mass"] = mass;
	return data;
}

void CP_RigidBody2::Load(const nlohmann::json& _data)
{
	Component::Load(_data);
	float loadMass = 1.0f;
	if (LoadJsonFloat("mass", loadMass, _data))
		SetMass(loadMass);

}

void CP_RigidBody2::GetBtTransform(btTransform& _btTrans)
{
	if (collider)
		collider->pMotionState->getWorldTransform(_btTrans);
}

void CP_RigidBody2::FindSetCollider()
{
	CP_Collider* pCol = GetGameObject().GetComponent<CP_Collider>();
	if (!pCol) return;

	SetColliderShape(*pCol);
}

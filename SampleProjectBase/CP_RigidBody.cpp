#include "pch.h"
#include "CP_RigidBody.h"

#include "DX11BulletPhisics.h"
#include "InSceneSystemManager.h"
#include "GameObject.h"

namespace DX = DirectX::SimpleMath;
using namespace HashiTaku;

constexpr float DEFAULT_MERGIN(0.04f);	// デフォルトのコリジョン形状マージン

CP_RigidBody::CP_RigidBody()
	: pShape(nullptr), mass(1.0f), isRigidbody(true), isGravity(true), isSetShape(false)
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
	// ゲームオブジェクトに伝える
	gameObject->SetRigidBody(true);
	// シーン内のRigidBody管理に追加
	InSceneSystemManager::GetInstance()->AddRigidBody(*this);
}

void CP_RigidBody::Update()
{
}

void CP_RigidBody::ImGuiSetting()
{
	if (!isSetShape) return;

	btTransform& bullet = pCollisionObject->getWorldTransform();
	DX::Vector3 pos = Bullet::ToDXVector3(bullet.getOrigin());
	DX::Quaternion quaternion = Bullet::ToDXQuaternion(bullet.getRotation());

	ImGuiMethod::Text(pos);
	ImGuiMethod::Text(Quat::ToEulerAngles(quaternion));

	float changeMass = mass;
	if (ImGui::DragFloat("mass", &changeMass, 0.1f, 0.0f, 500.0f))
		SetMass(changeMass);

	bool changeGravity = isGravity;
	if (ImGui::Checkbox("gravity", &changeGravity))
		SetIsGravity(changeGravity);
}

void CP_RigidBody::OnDestroy()
{
	RemoveCollObject();

	// ゲームオブジェクトに伝える
	if (gameObject)
		gameObject->SetRigidBody(false);

	InSceneSystemManager::GetInstance()->RemoveRigidBody(*this);
}

void CP_RigidBody::Start()
{
	SetMass(mass);
	SetIsGravity(isGravity);
}

void CP_RigidBody::OnEnableTrue()
{
	// ワールドに追加
	AddCollisionToWorld();
	gameObject->SetRigidBody(true);
	InSceneSystemManager::GetInstance()->AddRigidBody(*this);
}

void CP_RigidBody::OnEnableFalse()
{
	// ワールドから削除
	if (pCollisionObject)
		DX11BulletPhisics::GetInstance()->RemoveCollObj(*pCollisionObject);

	gameObject->SetRigidBody(false);
	InSceneSystemManager::GetInstance()->RemoveRigidBody(*this);
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

	if (!isSetShape) return;

	// 移動するオブジェクトかどうか
	bool isDynamic = mass > Mathf::epsilon;

	// 慣性モーメント再計算
	if (isDynamic)
	{
		btVector3 btInertia;
		pShape->calculateLocalInertia(mass, btInertia);
		inertia = Bullet::ToDXVector3(btInertia);
	}
	else
		inertia = DX::Vector3::Zero;

	if (isRigidbody)	// 剛体だったら
	{
		btRigidBody* pRigidBody = static_cast<btRigidBody*>(pCollisionObject.get());

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

	btTransform& bullettTransform = pCollisionObject->getWorldTransform();
	ToBtTransform(bullettTransform);
}

void CP_RigidBody::SetTransformBtToDx()
{
	if (!isSetShape) return;
	Transform& dxTransform = GetTransform();

	btTransform& bullettTransform = pCollisionObject->getWorldTransform();
	dxTransform.SetPosition(Bullet::ToDXVector3(bullettTransform.getOrigin()));
	dxTransform.SetRotation(Bullet::ToDXQuaternion(bullettTransform.getRotation()));
}

nlohmann::json CP_RigidBody::Save()
{
	auto data = Component::Save();
	data["mass"] = mass;
	data["isRigid"] = isRigidbody;
	data["isGravity"] = isGravity;
	return data;
}

void CP_RigidBody::Load(const nlohmann::json& _data)
{
	Component::Load(_data);
	LoadJsonFloat("mass", mass, _data);
	LoadJsonBoolean("isRigid", isRigidbody, _data);
	LoadJsonBoolean("isGravity", isGravity, _data);
}

void CP_RigidBody::Copy(const CP_RigidBody& _other)
{
	if (this == &_other) return;

	Component::operator=(_other);
	mass = _other.mass;
	SetIsRigidbody(_other.isRigidbody);
	SetIsGravity(_other.isGravity);

}

void CP_RigidBody::SetIsRigidbody(bool _isRB)
{
	// 変更前と同じなら
	if (isRigidbody == _isRB) return;

	isRigidbody = _isRB;
	CreateCollObject();	// 当たり判定を作成しなおす
}

void CP_RigidBody::SetIsGravity(bool _isGravity)
{
	if (!isSetShape) return;
	isGravity = _isGravity;

	if (!isRigidbody) return;
	btRigidBody* btRb = static_cast<btRigidBody*>(pCollisionObject.get());
	btRb->activate();

	if (isGravity)
	{
		DX::Vector3 gravityVal = DX11BulletPhisics::GetInstance()->GetGravityValue();
		btRb->setGravity(Bullet::ToBtVector3(gravityVal));
	}
	else
	{
		btRb->setGravity(btVector3(0, 0, 0));
		btRb->setLinearVelocity(btVector3(0, 0, 0));
		btRb->setAngularVelocity(btVector3(0, 0, 0));
	}
}

void CP_RigidBody::RemoveCollObject()
{
	// ワールドから削除
	if (!pCollisionObject) return;

	DX11BulletPhisics::GetInstance()->RemoveCollObj(*pCollisionObject);
	pCollisionObject.reset();
}

void CP_RigidBody::CreateCollObject()
{
	if (!isSetShape)
	{
		HASHI_DEBUG_LOG("先に形状を設定してください");
		return;
	}
	if (pCollisionObject)
	{
		// ワールドから削除
		DX11BulletPhisics::GetInstance()->RemoveCollObj(*pCollisionObject);
	}

	if (isRigidbody)	// 剛体へ
	{
		CreateRB();

	}
	else // Ghostへ
	{
		CreateGhost();
	}

	// ワールドに当たり判定を追加
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

	// 初速度を0にする
	btRigidBody* btRb = static_cast<btRigidBody*>(pCollisionObject.get());
	btRb->setLinearVelocity(btVector3(0, 0, 0));
	btRb->setAngularVelocity(btVector3(0, 0, 0));
}

void CP_RigidBody::CreateGhost()
{
	
}

void CP_RigidBody::CollisionSetTransform()
{
	if (!isSetShape) return;

	btTransform bulletTransform;
	ToBtTransform(bulletTransform);
	pCollisionObject->setWorldTransform(bulletTransform);
}

btRigidBody* CP_RigidBody::CastRigidBody()
{
	if (!isRigidbody || !isSetShape) return nullptr;

	return static_cast<btRigidBody*>(pCollisionObject.get());
}

void CP_RigidBody::AddCollisionToWorld()
{
	// ゲームオブジェクトのレイヤーをIDとする
	int groupId = static_cast<int>(gameObject->GetLayer().GetType());

	if(GetIsActive() && pCollisionObject)
		DX11BulletPhisics::GetInstance()->AddCollObj(*pCollisionObject, groupId);
}

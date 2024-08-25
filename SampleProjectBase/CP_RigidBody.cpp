#include "pch.h"
#include "CP_RigidBody.h"

#include "DX11BulletPhisics.h"

namespace DX = DirectX::SimpleMath;
using namespace HashiTaku;

CP_RigidBody::CP_RigidBody()
	: pShape(nullptr), mass(1.0f), isRigidBody(true), isSetShape(false)
{	
}

CP_RigidBody::CP_RigidBody(const CP_RigidBody& _other) 
	: pShape(nullptr), isSetShape(false)
{
	Copy(_other);
}

CP_RigidBody::~CP_RigidBody()
{
	// ワールドから削除
	if(pCollisionObject)
	DX11BulletPhisics::GetInstance()->RemoveCollObj(*pCollisionObject);
}

CP_RigidBody& CP_RigidBody::operator=(const CP_RigidBody& _other)
{
	Copy(_other);

	return *this;
}

void CP_RigidBody::Awake()
{
	btTransform bulletTransform;
	ToBtTransform(bulletTransform);
	pMotionState = std::make_unique<btDefaultMotionState>(bulletTransform);
}

void CP_RigidBody::LateUpdate()
{
	// RigidBodyのトランスフォームを更新する
	UpdateBtTransform();
}

void CP_RigidBody::ImGuiSetting()
{
	if (!isSetShape) return;

	btTransform& bullet = pCollisionObject->getWorldTransform();
	DX::Vector3 pos = Bullet::ToDXVector3(bullet.getOrigin());
	DX::Quaternion quaternion = Bullet::ToDXQuaternion(bullet.getRotation());

	ImGuiMethod::Text(pos);
	ImGuiMethod::Text(Quat::ToEulerAngles(quaternion));
}

void CP_RigidBody::Start()
{
	CreateCollObject();
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
}

void CP_RigidBody::SetShape(btCollisionShape& _shape)
{
	isSetShape = true;
	pShape = &_shape;

	CreateCollObject();
}

void CP_RigidBody::SetCollOffsetPos(const DirectX::SimpleMath::Vector3& _offsetPos)
{
	collOffsetPos = _offsetPos;
}

void CP_RigidBody::SetCollAngles(const DirectX::SimpleMath::Vector3& _offsetAngles)
{
	collOffsetAngle = _offsetAngles;
}

void CP_RigidBody::RemoveShape()
{
	isSetShape = false;
	pShape = nullptr;
}

void CP_RigidBody::Copy(const CP_RigidBody& _other)
{
	if (this == &_other) return;

	Component::operator=(_other);

	mass = _other.mass;
	isRigidBody = _other.isRigidBody;
}

void CP_RigidBody::SetRigidBody(bool _isRB)
{
	// 変更前と同じなら
	if (isRigidBody == _isRB) return;

	isRigidBody = _isRB;

	// 当たり判定を作成しなおす
	CreateCollObject();
}

void CP_RigidBody::CreateCollObject()
{
	if (!isSetShape)
	{
		HASHI_DEBUG_LOG("先に形状を設定してください");
		return;
	}
	if(pCollisionObject)
	{
		// ワールドから削除
		DX11BulletPhisics::GetInstance()->RemoveCollObj(*pCollisionObject);
	}

	if (isRigidBody)	// 剛体へ
	{
		CreateRB();

	}
	else // Ghostへ
	{
		CreateGhost();
	}

	// 物理ワールドに追加
	DX11BulletPhisics::GetInstance()->AddCollObj(*pCollisionObject);
}

void CP_RigidBody::CreateRB()
{
	btRigidBody::btRigidBodyConstructionInfo rbInfo(
		Bullet::ToBtScalar(mass), 
		pMotionState.get(),
		pShape,
		Bullet::ToBtVector3(inertia)
	);

	pCollisionObject = std::make_unique<btRigidBody>(rbInfo);
}

void CP_RigidBody::CreateGhost()
{
}

void CP_RigidBody::UpdateBtTransform()
{
	if (!isSetShape) return;

	btTransform bulletTransform;
	ToBtTransform(bulletTransform);

	pCollisionObject->setWorldTransform(bulletTransform);
	pMotionState->setWorldTransform(bulletTransform);
}

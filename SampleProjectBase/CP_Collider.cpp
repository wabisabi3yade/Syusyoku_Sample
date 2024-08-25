#include "pch.h"
#include "CP_Collider.h"
#include "Tag.h"

#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

const Color CP_Collider::normalColor = { 1,1,1,0.8f };
const Color CP_Collider::hitColor = { 1,0,0,1 };

CP_Collider::CP_Collider(Type _type) : type(_type)
{
}

CP_Collider::CP_Collider(const CP_Collider& _other)
{
	Copy(_other);
}

CP_Collider::~CP_Collider()
{
}

CP_Collider& CP_Collider::operator=(const CP_Collider& _other)
{
	Copy(_other);

	return *this;
}

void CP_Collider::Start()
{
	CreateRigidBody();
}

void CP_Collider::Copy(const CP_Collider& _other)
{
	if (this == &_other) return;

	Component::operator=(_other);
}

void CP_Collider::CreateRigidBody()
{
	CP_RigidBody* pRb = gameObject->GetComponent<CP_RigidBody>();

	if (!pRb)	// –³‚¯‚ê‚Î
	{
		// ’Ç‰Á‚·‚é
		gameObject->AddComponent<CP_RigidBody>();
	}
}

void CP_Collider::SendShapeToRb(btCollisionShape& _shape)
{
	CP_RigidBody* pRb = gameObject->GetComponent<CP_RigidBody>();
	assert(pRb != nullptr && "CP_RigidBody‚ªnull‚Å‚·");

	pRb->SetShape(_shape);
}

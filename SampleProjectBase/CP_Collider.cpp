#include "pch.h"
#include "CP_Collider.h"
#include "Tag.h"

#include "InSceneSystemManager.h"

namespace DX = DirectX::SimpleMath;
using namespace HashiTaku;

CP_Collider::CP_Collider(Type _type) : type(_type)
{
}

CP_Collider::CP_Collider(const CP_Collider& _other)
{
	Copy(_other);
}

CP_Collider& CP_Collider::operator=(const CP_Collider& _other)
{
	Copy(_other);

	return *this;
}

void CP_Collider::Init()
{
	pCompound = std::make_unique<btCompoundShape>();

	CreateShape();

	AddToCompound();

	SendShapeToRb();
}

void CP_Collider::OnDestroy()
{
	RemoveShapeFromRb();
}

void CP_Collider::SetCenterOffset(const DirectX::SimpleMath::Vector3& _offset)
{
	centerOffset = _offset;
	RemoveFromCompound();
	AddToCompound();
}

void CP_Collider::SetAngleOffset(const DirectX::SimpleMath::Vector3& _offset)
{
	angleOffset = _offset;
	RemoveFromCompound();
	AddToCompound();
}

btCollisionShape& CP_Collider::GetColliderShape()
{
	return *pCompound;
}

void CP_Collider::ImGuiSetting()
{
	if (ImGui::Button("Set Shape"))
		SettingShape();

	bool isChange = false;
	bool isAngChange = false;
	isChange = ImGuiMethod::DragFloat3(centerOffset, "center", 0.1f);
	isAngChange = ImGuiMethod::DragFloat3(angleOffset, "angles");

	if (isChange || isAngChange)
	{
		RemoveFromCompound();
		AddToCompound();
	}
}

nlohmann::json CP_Collider::Save()
{
	auto data = Component::Save();

	SaveJsonVector3("center", centerOffset, data);
	SaveJsonVector3("angles", angleOffset, data);

	return data;
}

void CP_Collider::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	LoadJsonVector3("center", centerOffset, _data);
	LoadJsonVector3("angles", angleOffset, _data);
}

void CP_Collider::Copy(const CP_Collider& _other)
{
	if (this == &_other) return;

	Component::operator=(_other);
}

void CP_Collider::RemoveShapeFromRb()
{
	if (!gameObject) return;

	CP_RigidBody2* pRb = gameObject->GetComponent<CP_RigidBody2>();
	if (pRb)
		pRb->RemoveColliderShape(*this);
}

void CP_Collider::OnEnableTrue()
{
	SendShapeToRb();
}

void CP_Collider::OnEnableFalse()
{
	RemoveShapeFromRb();
}

void CP_Collider::OnChangeTransform()
{
	RemoveFromCompound();
	AddToCompound();
}

void CP_Collider::SettingShape()
{
	RemoveFromCompound();

	CreateShape();

	AddToCompound();
}

void CP_Collider::SendShapeToRb()
{
	CP_RigidBody2* pRb = gameObject->GetComponent<CP_RigidBody2>();
	if (!pRb)
	{
		HASHI_DEBUG_LOG("RigidBody‚ª‚ ‚è‚Ü‚¹‚ñ");
		return;
	}

	pRb->SetColliderShape(*this);
}

void CP_Collider::RemoveFromCompound()
{
	if (pCollisionShape)
		pCompound->removeChildShape(pCollisionShape.get());
}

void CP_Collider::AddToCompound()
{
	using namespace DirectX::SimpleMath;

	if (pCollisionShape)
	{
		btTransform btTrans;
		btTrans.setIdentity();

		Transform& transform = GetTransform();
		Vector3 worldOffset = GetTransform().GetScale() * centerOffset;
		btTrans.setOrigin(Bullet::ToBtVector3(worldOffset));

		Vector3 worldAngles = transform.GetEularAngles() + angleOffset;
		btTrans.setRotation(Bullet::ToBtQuaeternion(Quat::ToQuaternion(worldAngles)));

		pCompound->addChildShape(btTrans , pCollisionShape.get());
	}
	else
	{
		HASHI_DEBUG_LOG("Œ`ó‚ğæ‚Éì¬‚µ‚Ä‚­‚¾‚³‚¢");
	}
}

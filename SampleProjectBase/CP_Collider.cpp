#include "pch.h"
#include "CP_Collider.h"
#include "Tag.h"

#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

const Color CP_Collider::normalColor = { 1,1,1,0.8f };
const Color CP_Collider::hitColor = { 1,0,0,1 };

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
	//pCompound = std::make_unique<btCompoundShape>();

	SetShape();
	/*SendShapeToRb();*/
}

void CP_Collider::OnDestroy()
{
	RemoveShapeOfRb();
}

void CP_Collider::SetCenterOffset(const DirectX::SimpleMath::Vector3& _offset)
{
	centerOffset = _offset;
	SetShape();
}

void CP_Collider::SetAngleOffset(const DirectX::SimpleMath::Vector3& _offset)
{
	angleOffset = _offset;
	SetShape();
}

void CP_Collider::ImGuiSetting()
{
	if (ImGui::Button("Set Shape"))
		SendShapeToRb();

	Vector3 p_centerPos = centerOffset;
	Vector3 p_offsetAngles = angleOffset;

	ImGuiMethod::DragFloat3(centerOffset, "center", 0.1f);
	ImGuiMethod::DragFloat3(angleOffset, "angles");

	// •ÏX‚µ‚Ä‚¢‚é‚È‚ç
	if (centerOffset != p_centerPos || angleOffset != p_offsetAngles)
		SetShape();
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

void CP_Collider::RemoveShapeOfRb()
{
	if (!gameObject) return;

	CP_RigidBody* pRb = gameObject->GetComponent<CP_RigidBody>();
	if (pRb)
		pRb->RemoveShape();
}

void CP_Collider::OnEnableTrue()
{
}

void CP_Collider::OnEnableFalse()
{
}

void CP_Collider::SetShape()
{
	//if (pCollisionShape)
	//	pCompound->removeChildShape(pCollisionShape.get());

	if (pCollisionShape)
	{
		CP_RigidBody* pRb = gameObject->GetComponent<CP_RigidBody>();
		if (pRb)
			pRb->RemoveShape();
	}

	CreateShape();

	btTransform offsetTransform;
	offsetTransform.setIdentity();
	offsetTransform.setOrigin(Bullet::ToBtVector3(centerOffset));
	offsetTransform.setRotation(Bullet::ToBtQuaeternion(Quat::ToQuaternion(angleOffset)));


	SendShapeToRb();
	/*pCompound->addChildShape(offsetTransform, pCollisionShape.get());*/
}

void CP_Collider::SendShapeToRb()
{
	CP_RigidBody* pRb = gameObject->GetComponent<CP_RigidBody>();
	if (!pRb)
	{
		HASHI_DEBUG_LOG("RigidBody‚ª‚ ‚è‚Ü‚¹‚ñ");
		return;
	}

	pRb->SetShape(/**pCompound*/*pCollisionShape);
}

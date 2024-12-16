#include "pch.h"
#include "CP_Collider.h"
#include "Tag.h"

#include "InSceneSystemManager.h"

namespace HashiTaku
{
	CP_Collider::CP_Collider(ShapeType _type) : type(_type), isCreateCompound(false)
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
		isCreateCompound = true;

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

	CP_Collider::ShapeType CP_Collider::GetType() const
	{
		return type;
	}

	bool CP_Collider::GetIsCreateCompound() const
	{
		return isCreateCompound;
	}

	void CP_Collider::ImGuiDebug()
	{
		if (ImGui::Button("Set Shape"))
			RecreateShape();

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

		// RecreateShapeは各派生コライダーに描く
	}

	void CP_Collider::Copy(const CP_Collider& _other)
	{
		if (this == &_other) return;

		Component::operator=(_other);
	}

	void CP_Collider::RemoveShapeFromRb()
	{
		if (!gameObject) return;

		CP_RigidBody* pRb = gameObject->GetComponent<CP_RigidBody>();
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

	void CP_Collider::OnChangeScale()
	{
		RecreateShape();
	}

	void CP_Collider::RecreateShape()
	{
		// コリジョン形状を一旦削除
		RemoveFromCompound();

		// 形状をもう一度作成
		CreateShape();

		// コンパウンドに追加
		AddToCompound();
	}

	void CP_Collider::SendShapeToRb()
	{
		CP_RigidBody* pRb = gameObject->GetComponent<CP_RigidBody>();
		if (!pRb)
		{
			HASHI_DEBUG_LOG("RigidBodyがありません");
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
			btTrans.setRotation(Bullet::ToBtQuaeternion(Quat::ToQuaternion(angleOffset)));

			pCompound->addChildShape(btTrans, pCollisionShape.get());
		}
		else
		{
			HASHI_DEBUG_LOG("形状を先に作成してください");
		}
	}
}
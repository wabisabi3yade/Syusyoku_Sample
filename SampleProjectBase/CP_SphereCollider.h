#pragma once
#include "CP_Collider.h"

class CP_SphereCollider : public CP_Collider, public CloneComponent<CP_SphereCollider>
{
	// 半径
	float radius;	
	// オフセット
	DirectX::SimpleMath::Vector3 center;	
public:
	CP_SphereCollider() : CP_Collider(CP_Collider::Type::Sphere), radius(0.5f) {}
	~CP_SphereCollider(){};

	void Start() override;
	
	void Draw() override;
	void ImGuiSetting() override;

	float GetRadius() { return radius; }
	DirectX::SimpleMath::Vector3 GetCenterPos()const;

	static bool CollisionSphere(CP_Collider& _sphere1, CP_Collider& _sphere2);
	static bool CollisionBox(CP_Collider& _sphere, CP_Collider& _box);

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
};


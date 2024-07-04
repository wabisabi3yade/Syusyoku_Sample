#pragma once
#include "CP_Collider.h"

class SphereCollider : public CP_Collider
{
	// 半径
	float radius;	
	// オフセット
	DirectX::SimpleMath::Vector3 posOffset;	
public:
	using CP_Collider::CP_Collider;
	SphereCollider& operator=(const SphereCollider& _other);
	~SphereCollider(){};

	void Init() override;
	
	void Draw() override;
	void ImGuiSetting() override;

	float GetRadius() { return radius; }
	DirectX::SimpleMath::Vector3 GetCenterPos()const;

	static bool CollisionSphere(CP_Collider& _sphere1, CP_Collider& _sphere2);
	static bool CollisionBox(CP_Collider& _sphere, CP_Collider& _box);
};


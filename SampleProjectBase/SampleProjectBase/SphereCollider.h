#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
	// 半径
	float radius;	
	// オフセット
	DirectX::SimpleMath::Vector3 posOffset;	
public:
	using Collider::Collider;
	SphereCollider& operator=(const SphereCollider& _other);
	~SphereCollider(){};

	void Init() override;
	
	void Draw() override;
	void ImGuiSetting() override;

	float GetRadius() { return radius; }
	DirectX::SimpleMath::Vector3 GetCenterPos()const;

	static bool CollisionSphere(Collider& _sphere1, Collider& _sphere2);
	static bool CollisionBox(Collider& _sphere, Collider& _box);
};


#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
	float radius;	// ���a
	DirectX::SimpleMath::Vector3 posOffset;	// ���S���W	
public:
	using Collider::Collider;

	~SphereCollider();

	void Init() override;
	
	void Draw() override;
	void SetParameter() override;

	float GetRadius() { return radius; }
	DirectX::SimpleMath::Vector3 GetCenterPos()const;

	static bool CollisionSphere(Collider& sphere1, Collider& sphere2);
};


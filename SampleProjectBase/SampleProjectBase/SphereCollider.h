#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
	// ���a
	float radius;	
	// �I�t�Z�b�g
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

	static bool CollisionSphere(Collider& sphere1, Collider& sphere2);
};


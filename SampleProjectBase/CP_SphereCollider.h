#pragma once
#include "CP_Collider.h"

class CP_SphereCollider : public CP_Collider, public CloneComponent<CP_SphereCollider>
{
	// ���a
	float radius;	
	// �I�t�Z�b�g
	DirectX::SimpleMath::Vector3 posOffset;	
public:
	CP_SphereCollider() : CP_Collider(CP_Collider::Type::Sphere), radius(0.5f) {}
	~CP_SphereCollider(){};
	
	void Draw() override;
	void ImGuiSetting() override;

	float GetRadius() { return radius; }
	DirectX::SimpleMath::Vector3 GetCenterPos()const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	void CreateShape() override;
	/// @brief ���a���Z�b�g
	void SetRadius(float _radius);
};


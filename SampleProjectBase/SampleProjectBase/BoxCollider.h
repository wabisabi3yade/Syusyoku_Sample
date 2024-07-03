#pragma once
#include "Collider.h"

class BoxCollider :
	public Collider
{
	// ���W�I�t�Z�b�g
	DirectX::SimpleMath::Vector3 posOffset;
	// �X��
	DirectX::SimpleMath::Vector3 angleOffset;  
	// �傫��
	DirectX::SimpleMath::Vector3 size; 
	// AABB�t���O
	bool isAABB = false;	

	static void CheckCollisionAABB(Collider& _box1, Collider& _box2);
	static void CheckCollisionOBB(Collider& _box1, Collider& _box2);
public:
	using Collider::Collider;
	BoxCollider& operator=(const BoxCollider& _other);

	void Init();
	void Draw() override;

	void ImGuiSetting() override;

	void SetOffset(const DirectX::SimpleMath::Vector3& _offset) { posOffset = _offset; } 
	void SetAngle(const DirectX::SimpleMath::Vector3& _angle) { angleOffset = _angle; }    // �p�x
	void SetSize(const DirectX::SimpleMath::Vector3& _size) { size = _size; } // �傫��

	// ���[���h���W�n�֕ϊ�
	DirectX::SimpleMath::Vector3 GetWorldCenterPos()const;
	DirectX::SimpleMath::Vector3 GetWorldRotation()const;
	DirectX::SimpleMath::Vector3 GetWorldScale()const;

	static bool CollisionBox(Collider& _box1, Collider& _box2);
};


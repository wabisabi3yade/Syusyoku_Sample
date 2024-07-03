#pragma once
#include "Collider.h"

class BoxCollider :
	public Collider
{
	// 座標オフセット
	DirectX::SimpleMath::Vector3 posOffset;
	// 傾き
	DirectX::SimpleMath::Vector3 angleOffset;  
	// 大きさ
	DirectX::SimpleMath::Vector3 size; 
	// AABBフラグ
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
	void SetAngle(const DirectX::SimpleMath::Vector3& _angle) { angleOffset = _angle; }    // 角度
	void SetSize(const DirectX::SimpleMath::Vector3& _size) { size = _size; } // 大きさ

	// ワールド座標系へ変換
	DirectX::SimpleMath::Vector3 GetWorldCenterPos()const;
	DirectX::SimpleMath::Vector3 GetWorldRotation()const;
	DirectX::SimpleMath::Vector3 GetWorldScale()const;

	static bool CollisionBox(Collider& _box1, Collider& _box2);
};


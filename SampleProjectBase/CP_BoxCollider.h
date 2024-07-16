#pragma once
#include "CP_Collider.h"

class CP_BoxCollider :
	public CP_Collider
{
	// 座標オフセット
	DirectX::SimpleMath::Vector3 posOffset;
	// 傾き
	DirectX::SimpleMath::Vector3 angleOffset;  
	// 大きさ
	DirectX::SimpleMath::Vector3 size; 
	// AABBフラグ
	bool isAABB = false;	

	static void CheckCollisionAABB(CP_Collider& _box1, CP_Collider& _box2);
	static void CheckCollisionOBB(CP_Collider& _box1, CP_Collider& _box2);
public:
	using CP_Collider::CP_Collider;
	CP_BoxCollider& operator=(const CP_BoxCollider& _other);

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

	static bool CollisionBox(CP_Collider& _box1, CP_Collider& _box2);

private:

	/// @brief モデルのサイズから当たり判定の大きさを決める
	void SizeFromModelSize();
};


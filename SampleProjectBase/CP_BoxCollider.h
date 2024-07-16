#pragma once
#include "CP_Collider.h"

class CP_BoxCollider :
	public CP_Collider
{
	// ���W�I�t�Z�b�g
	DirectX::SimpleMath::Vector3 posOffset;
	// �X��
	DirectX::SimpleMath::Vector3 angleOffset;  
	// �傫��
	DirectX::SimpleMath::Vector3 size; 
	// AABB�t���O
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
	void SetAngle(const DirectX::SimpleMath::Vector3& _angle) { angleOffset = _angle; }    // �p�x
	void SetSize(const DirectX::SimpleMath::Vector3& _size) { size = _size; } // �傫��

	// ���[���h���W�n�֕ϊ�
	DirectX::SimpleMath::Vector3 GetWorldCenterPos()const;
	DirectX::SimpleMath::Vector3 GetWorldRotation()const;
	DirectX::SimpleMath::Vector3 GetWorldScale()const;

	static bool CollisionBox(CP_Collider& _box1, CP_Collider& _box2);

private:

	/// @brief ���f���̃T�C�Y���瓖���蔻��̑傫�������߂�
	void SizeFromModelSize();
};


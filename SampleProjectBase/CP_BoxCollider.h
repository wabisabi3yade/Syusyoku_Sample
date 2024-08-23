#pragma once
#include "CP_Collider.h"

class CP_BoxCollider : public CP_Collider, public CloneComponent<CP_BoxCollider>
{
	/// @brief ���S���W
	DirectX::SimpleMath::Vector3 centerPos;

	/// @brief ����
	DirectX::SimpleMath::Vector3 length;
	
	/// @brief �E�����x�N�g��
	DirectX::SimpleMath::Vector3 rightVector;

	/// @brief ������x�N�g��
	DirectX::SimpleMath::Vector3 upVector;

	/// @brief �O�����x�N�g��
	DirectX::SimpleMath::Vector3 forwardVector;

	/// @brief ���W�I�t�Z�b�g
	DirectX::SimpleMath::Vector3 posOffset;

	//// �X��
	//DirectX::SimpleMath::Vector3 angleOffset;  

	// �傫���I�t�Z�b�g
	DirectX::SimpleMath::Vector3 sizeOffset; 

	// AABB�t���O
	bool isAABB = false;	

public:
	CP_BoxCollider();
	~CP_BoxCollider() {}

	void Start();

	void LateUpdate() override;

	void Draw() override;

	void ImGuiSetting() override;

	void SetOffset(const DirectX::SimpleMath::Vector3& _offset) { posOffset = _offset; } 
	//void SetAngle(const DirectX::SimpleMath::Vector3& _angle) { angleOffset = _angle; }    // �p�x
	void SetSize(const DirectX::SimpleMath::Vector3& _size) { sizeOffset = _size; } // �傫��

	// ���[���h���W�n�֕ϊ�
	DirectX::SimpleMath::Vector3 GetCenterPos()const;
	DirectX::SimpleMath::Vector3 GetLength()const;

	// �e�����x�N�g��
	DirectX::SimpleMath::Vector3 GetRightVector()const;
	DirectX::SimpleMath::Vector3 GetUpVector()const;
	DirectX::SimpleMath::Vector3 GetForwardVector()const;

	static bool CollisionBox(CP_Collider& _box1, CP_Collider& _box2);

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	/// @brief ���f���̃T�C�Y���瓖���蔻��̑傫�������߂�
	void SizeFromModelSize();

	/// @brief ���W�X�V
	void PosUpdate();
	
	/// @brief �����X�V
	void VectorUpdate();

	/// @brief �����X�V
	void LengthUpdate();
};


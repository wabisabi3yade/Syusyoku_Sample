#pragma once
#include "CP_Collider.h"

class CP_BoxCollider : public CP_Collider, public CloneComponent<CP_BoxCollider>
{
	/// @brief ���S���W
	DirectX::SimpleMath::Vector3 centerPos;

	/// @brief ����
	DirectX::SimpleMath::Vector3 length;

	/// @brief ���W�I�t�Z�b�g
	DirectX::SimpleMath::Vector3 posOffset;

	// AABB�t���O
	bool isAABB = false;	

public:
	CP_BoxCollider();
	~CP_BoxCollider() {}

	void LateUpdate() override;

	void ImGuiSetting() override;

	void SetOffset(const DirectX::SimpleMath::Vector3& _offset) { posOffset = _offset; } 

	// ���[���h���W�n�֕ϊ�
	DirectX::SimpleMath::Vector3 GetCenterPos()const;
	DirectX::SimpleMath::Vector3 GetLength()const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	void Start();
	/// @brief �{�b�N�X�̌`����쐬
	void CreateBoxShape();

	/// @brief ���f���̃T�C�Y���瓖���蔻��̑傫�������߂�
	void SizeFromModelSize();

	/// @brief ���W�X�V
	void PosUpdate();

	/// @brief �����X�V
	void LengthUpdate();
};


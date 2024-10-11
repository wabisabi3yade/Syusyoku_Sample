#pragma once
#include "CP_Collider.h"

class CP_BoxCollider : public CP_Collider
{
	/// @brief ����
	DirectX::SimpleMath::Vector3 length;

	// AABB�t���O
	bool isAABB = false;	

public:
	CP_BoxCollider();
	~CP_BoxCollider() {}

	void ImGuiSetting() override;

	void SetLength(const DirectX::SimpleMath::Vector3& _length);

	DirectX::SimpleMath::Vector3 GetLength()const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	void Init();
	void LateUpdate() override;

	/// @brief �{�b�N�X�̌`����쐬
	void CreateShape() override;

	/// @brief ���f���̃T�C�Y���瓖���蔻��̑傫�������߂�
	void SizeFromModelSize();

	/// @brief �����X�V
	void LengthUpdate();

	/// @brief ���[���h���W�Ɍv�Z
	/// @param ���[���h���W�ł̒���
	void CalcWorldLength(DirectX::SimpleMath::Vector3& _out);
};


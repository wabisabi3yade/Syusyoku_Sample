#pragma once
#include "CP_Collider.h"

/// @brief �J�v�Z���R���C�_�[
class CP_CapsuleCollider : public CP_Collider
{
	/// @brief ���a
	float radius;

	/// @brief ����
	float height;
public:
	CP_CapsuleCollider();
	~CP_CapsuleCollider() {}

	/// @brief ���a���Z�b�g
	/// @param _setRadius ���a
	void SetRadius(float _setRadius);

	/// @brief �������Z�b�g
	/// @param _setHeight ����
	void SetHeight(float _setHeight);

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �{�b�N�X�̌`����쐬
	void CreateShape() override;

	/// @brief �I�u�W�F�N�g�̃X�P�[���l�𔽉f
	/// @param _outRadius ���f�������a
	/// @param _outHeight ���f��������
	void ApplyObjectScale(float& _outRadius, float& _outHeight);

	void ImGuiSetting() override;
};


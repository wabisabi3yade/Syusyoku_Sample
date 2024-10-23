#pragma once
#include "CP_Collider.h"

class CP_SphereCollider : public CP_Collider
{
	// ���a
	float radius;	
public:
	CP_SphereCollider() : CP_Collider(CP_Collider::ShapeType::Sphere), radius(0.5f) {}
	~CP_SphereCollider(){};
	
	// �R���|�[�l���g���ʊ֐�
	void Draw() override;

	/// @brief ���a���擾����
	/// @return ���a
	float GetRadius() const;

	void ImGuiSetting() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �`��쐬
	void CreateShape() override;

	/// @brief ���a���Z�b�g
	void SetRadius(float _radius);

	/// @brief �I�u�W�F�N�g�̃X�P�[���l��K�p����
	/// @param _outRadius �v�Z�������a
	void ApplyObjectScale(float& _outRadius);
};


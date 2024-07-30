#pragma once
#include "Component.h"

class Camera;
class CameraMove : public Component
{
	const GameObject* pPlayer{ nullptr };	// �v���C���[�̃I�u�W�F�N�g

	// �J�����N���X
	Camera* camera{ nullptr };

	// �J�����̈ړ����x
	float moveSpeed{ 0.0f };

	/// @brief �����Ƃ��̑��x
	float dashSpeed{ 0.0f };

	// ���_�ړ����x
	float lookSpeed{ 0.0f };

	// �ړ������x�N�g��
	DirectX::SimpleMath::Vector3 moveVec;

	// ���_�ړ��x�N�g��
	DirectX::SimpleMath::Vector3 rotateVec;

public:
	using Component::Component;

	virtual void Init();
	virtual void LateUpdate();

	virtual void ImGuiSetting() override;

private:
	void UpdateVector();
	void Move();
};


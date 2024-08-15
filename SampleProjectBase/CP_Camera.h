#pragma once
#include "Component.h"

// �Q�[�����J�����N���X
class CP_Camera : public Component
{
	/// @brief �����_
	DirectX::SimpleMath::Vector3 focusPos{ 0.0f, 0.0f, 0.0f };

	/// @brief �J�����̏�x�N�g��
	DirectX::SimpleMath::Vector3 camUp{ Vec3::Up };

	/// @brief ����p
	float fov;

	/// @brief �`��ŒZ
	float nearZ;

	/// @brief �`��Œ�
	float farZ;

	/// @brief ���s���e�ɂ��邩�H
	bool isOrthographic{ false };

	// �r���[�|�[�g�̔ԍ�
	u_int viewPortSlot;
public:
	CP_Camera();
	~CP_Camera() {}

	void Init() override;

	void LateUpdate() override;

	void ImGuiSetting() override;

	/// @brief �r���[�ϊ��s��
	void UpdateViewMatrix();	

	/// @brief �������e�ɕύX
	void SetPerspective();

	/// @brief ���s���e�ɕύX
	void SetOrthographic();

	/// @brief �����_�Z�b�g
	/// @param _focusPos ���[���h���W
	void SetFocusPos(const DirectX::SimpleMath::Vector3& _focusPos);

	// �r���[�|�[�g�ԍ����Z�b�g
	void SetViewportSlot(u_int _slot);

private:
	/// @brief �������e�s����X�V
	void UpdatePerspective();

	/// @brief ���s���e���Z�b�g����
	void UpdateOrthographic();
};


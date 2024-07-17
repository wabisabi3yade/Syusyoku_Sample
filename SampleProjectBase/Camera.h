#pragma once
#include "Component.h"

// �Q�[�����J�����N���X
class Camera : public Component
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
public:
	using Component::Component;

	void Init() override;

	/// @brief �r���[�ϊ��s��
	void UpdateViewMatrix();	

	/// @brief �������e�ɕύX
	/// @param _viewPortSlot 
	void SetPerspective(u_int _viewPortSlot = 0);

	/// @brief ���s���e�ɕύX
	/// @param _viewPortSlot �r���[�|�[�g�C���f�b�N�X
	void SetOrthographic(u_int _viewPortSlot = 0);

	/// @brief �����_�Z�b�g
	/// @param _focusPos ���[���h���W
	void SetFocusPos(const DirectX::SimpleMath::Vector3& _focusPos);

private:
	/// @brief �������e�s����X�V
	/// @param _viewPortSlot �r���[�|�[�g�C���f�b�N�X
	void UpdatePerspective(u_int _viewPortSlot);

	/// @brief ���s���e���Z�b�g����
	/// @param _viewPortSlot �r���[�|�[�g�C���f�b�N�X
	void UpdateOrthographic(u_int _viewPortSlot);
};


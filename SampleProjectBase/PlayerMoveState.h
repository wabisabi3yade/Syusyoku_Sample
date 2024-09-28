#pragma once
#include "PlayerActState_Base.h"

/// @brief �v���C���[�ړ�����
class PlayerMoveState : public PlayerActState_Base
{
	/// @brief �ړ�����
	DirectX::SimpleMath::Vector3 moveVector;

	/// @brief ���݂̑��x
	float currentSpeed;

	/// @brief �ő呬�x
	float maxSpeed;

	/// @brief �����x
	float acceleration;

	/// @brief ��]���x
	float rotateSpeed;
public:
	PlayerMoveState();
	~PlayerMoveState() {}

private:
	void OnStart() override;
	void Update() override;
	void OnEnd() override;

	/// @brief �ړ�
	void Move();

	/// @brief ��]������
	void Rotation();

	/// @brief	���͒l��Ԃ�
	DirectX::SimpleMath::Vector2 InputValue();

	/// @brief �ړ����͂���Ă��邩
	bool IsMoveInput();

	void ImGuiSetting() override;
};


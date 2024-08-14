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
public:
	PlayerMoveState(PlayerActionController& _controller);
	~PlayerMoveState() {}

	void Init() override;

	void Terminal() override;

	void ImGuiSetting() override;

private:
	void Update() override;

	void TransitionCheck() override;

	/// @brief �ړ�
	void Move();

	/// @brief ��]������
	void Rotation();

	/// @brief	���͒l��Ԃ�
	DirectX::SimpleMath::Vector2 InputValue();

	/// @brief �ړ����͂���Ă��邩
	bool IsMoveInput();
};


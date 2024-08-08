#pragma once
#include "PlayerState_Base.h"

/// @brief �v���C���[�ړ�����
class PlayerMoveState : public PlayerState_Base
{
	DirectX::SimpleMath::Vector3 moveVector;

	float moveSpeed;

public:
	PlayerMoveState(PlayerActionController& _controller, GameObject& _gameObject);
	~PlayerMoveState() {}

	void Init() override;

	void Update() override;

	void Terminal() override;

private:
	void Move();
};


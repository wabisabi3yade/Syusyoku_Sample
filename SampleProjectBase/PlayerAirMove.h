#pragma once
#include "PlayerAirState.h"

/// @brief �󒆈ړ�
class PlayerAirMove : public PlayerAirState
{
	/// @brief �ړ�����
	DirectX::SimpleMath::Vector3 moveVector;

	/// @brief ���݂̑��x
	float currentSpeed;

	/// @brief �ő呬�x
	float maxSpeed;

	/// @brief �����x
	float acceleration;

	/// @brief �����X�s�[�h
	float decadeSpeed;

	/// @brief ��]���x
	float vectorRotateSpeed;

	/// @brief �W�����v�ŏオ�葱����ő厞��
	float maxJumpUpTime;

	/// @brief �X�e�[�g�J�n���̏�ɂނ����W�����v�X�s�[�h
	float enterJumpSpeed;
public:
	PlayerAirMove();
	~PlayerAirMove() {}

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	/// @brief �J�n����
	void OnStartBehavior() override;

	/// @brief �X�V
	void UpdateBehavior() override;

	/// @brief �ړ�
	void Move();

	/// @brief ��]������
	void Rotation();

	void ImGuiDebug() override;
};


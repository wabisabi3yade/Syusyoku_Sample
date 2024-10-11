#pragma once
#include "PlayerActState_Base.h"

class CP_Camera;

/// @brief �v���C���[�ړ�����
class PlayerMoveState : public PlayerActState_Base
{
	/// @brief �ړ�����
	DirectX::SimpleMath::Vector3 moveVector;

	CP_Camera* pCamera;

	/// @brief ���݂̑��x
	float currentSpeed;

	/// @brief �ő呬�x
	float maxSpeed;

	/// @brief �����x
	float acceleration;

	/// @brief ������
	float decadeSpeedTimes;

	/// @brief ��]���x
	float rotateSpeed;
public:
	PlayerMoveState();
	~PlayerMoveState() {}

	/// @brief �Z�[�u����
	/// @return �Z�[�u�f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data) override;
private:
	void OnStart() override;
	void Update() override;
	void OnEnd() override;

	/// @brief �ړ�
	void Move();

	DirectX::SimpleMath::Vector3 MoveVector();

	/// @brief ��]������
	void Rotation();

	/// @brief	���͒l��Ԃ�
	DirectX::SimpleMath::Vector2 InputValue();

	/// @brief �ړ����͂���Ă��邩
	bool IsMoveInput();

	/// @brief �����Ă��邩�擾
	/// @return �����Ă��邩�H
	bool IsRunning();

	void ImGuiSetting() override;
};


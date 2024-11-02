#pragma once
#include "PlayerActState_Base.h"
#include "AnimationCurve.h"

class CP_Camera;

/// @brief �v���C���[�ړ�����
class PlayerMoveState : public PlayerActState_Base
{
	/// @brief �ړ�����
	DirectX::SimpleMath::Vector3 moveVector;

	AnimationCurve curve;

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
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;
	void TransitionCheckUpdate() override;

	/// @brief �ړ�
	void Move();

	/// @brief ��]������
	void Rotation();

	/// @brief �ړ����͂���Ă��邩
	bool IsMoveInput();

	/// @brief �����Ă��邩�擾
	/// @return �����Ă��邩�H
	bool IsRunning();

	void ImGuiSetting() override;
};


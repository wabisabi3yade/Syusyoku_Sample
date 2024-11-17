#pragma once
#include "PlayerActState_Base.h"
#include "AnimationCurve.h"

class CP_Camera;

/// @brief �v���C���[�ړ�����
class PlayerMoveState : public PlayerActState_Base
{
protected:
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
	float rotateSpeed;
public:
	PlayerMoveState();
	virtual ~PlayerMoveState() {}

	/// @brief �Z�[�u����
	/// @return �Z�[�u�f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data) override;

protected:
	void TransitionCheckUpdate() override;

	/// @brief �ړ�
	void Move();

	/// @brief �����Ă��邩�擾
	/// @return �����Ă��邩�H
	bool IsRunning();

	void ImGuiDebug() override;
private:
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;

	/// @brief ���[�g���[�V�����K�p
	void ApplyRootMotion();

	/// @brief ��]������
	void Rotation();

	/// @brief �ړ����͂���Ă��邩
	bool IsMoveInput();
};


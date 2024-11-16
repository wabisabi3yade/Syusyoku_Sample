#pragma once
#include "CameraMoveState_Base.h"
#include "GameInput.h"

/// @brief �v���C���[�ړ����̃J�����̍s��
class CameraOnMoveState : public CameraMoveState_Base
{
	/// @brief �J�����|�W�V����
	DirectX::SimpleMath::Vector3 cameraPos;

	/// @brief �^�[�Q�b�g�̃��[���h���W
	DirectX::SimpleMath::Vector3 targetPos;

	/// @brief ���͐��l
	DirectX::SimpleMath::Vector2 inputVal;

	/// @brief �Q�[��������
	GameInput* pInput;

	/// @brief ���݂�y����
	float currentHeight;

	/// @brief �Ώۂ̃I�u�W�F�N�g�Ƃ̍ő�y����
	float cameraHeightMax;

	/// @brief �Ώۂ̃I�u�W�F�N�g�Ƃ̍ŏ�y����
	float cameraHeightMin;

	/// @brief �c�����̑��x
	float verticalSpeed;

	/// @brief ��]���x
	float rotateSpeed;

	/// @brief ���S�Ƃ̊p�x
	float centerAngle;

	/// @brief �����̑ΏۂƂ̃J��������
	float distanceHorizon;

	/// @brief �^�[�Q�b�g����Y���W�̒����_
	float lookTargetOffsetY;

public :
	CameraOnMoveState();
	~CameraOnMoveState() {}

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �J�n����
	void OnStartBehavior();

	/// @brief �X�V����
	void UpdateBehavior();

	/// @brief �I������
	void OnEndBehavior();

	/// @brief �c���̈ړ�
	void VerticalMove();

	/// @brief �ړ�����
	void RotationMove();

	/// @brief �����_���X�V
	void LookUpdate();
	
	void ImGuiDebug() override;
};


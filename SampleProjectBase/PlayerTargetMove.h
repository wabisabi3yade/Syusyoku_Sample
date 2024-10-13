#pragma once
#include "PlayerActState_Base.h"

class PlayerTargetMove : public PlayerActState_Base
{
	/// @brief �ړ�����
	DirectX::SimpleMath::Vector3 moveVector;

	/// @brief ���݂̑��x
	float currentSpeed;

	/// @brief �ő呬�x
	float maxSpeed;

	/// @brief �����x
	float acceleration;

	/// @brief ������
	float decadeSpeedTimes;
public:
	PlayerTargetMove();
	~PlayerTargetMove() {}

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

	/// @brief �����Ă��邩�擾
	/// @return �����Ă��邩�H
	bool IsRunning();

	void ImGuiSetting() override;

};


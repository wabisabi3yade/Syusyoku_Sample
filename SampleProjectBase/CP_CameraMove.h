#pragma once
#include "Component.h"

class CP_Camera;

class CP_CameraMove : public Component
{
	/// @brief �Ǐ]�Ώ�
	GameObject* pTargetObj;

	/// @brief �J����
	CP_Camera* pCamera;

	/// @brief �����_�̃I�t�Z�b�g
	DirectX::SimpleMath::Vector3 offsetTarget;

	/// @brief �J�����̈ړ����x	
	float rotateSpeed;

	/// @brief �^�[�Q�b�g�Ƃ̒��S�p�x
	float centerAngle;

	/// @brief �^�[�Q�b�g�Ƃ̉�����
	float distanceHori;

	/// @brief �^�[�Q�b�g�Ƃ̏c����
	float distanceVer;
	
	/// @brief ���_�ړ��x�N�g��
	float rotateVec;

public:
	CP_CameraMove();
	~CP_CameraMove() {}

	virtual void Start();
	virtual void LateUpdate();

	virtual void ImGuiDebug() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	void UpdateVector();
	void Move();
	void LookUpdate();

	bool IsCanUpdate();
};


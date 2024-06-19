#pragma once
#include "GameObject.h"

// �Q�[�����J�����N���X
class Camera : public GameObject
{
	// �J�����̒����_
	DirectX::SimpleMath::Vector3 focusPos{ 0,0,0 };
	// �J�����̏�x�N�g��
	DirectX::SimpleMath::Vector3 camUp{ 0,1.f,0 };

	float DEFAULT_FOV = 45.0f;
	float DEFAULT_NEARZ = 0.1f;
	float DEFAULT_FARZ = 1000.0f;

	void UpdateFocus();	// �����_�̍��W���J�����̊p�x����X�V����
public:
	Camera();
	~Camera();

	void LateUpdate() override;

	void UpdateViewMatrix();	// �r���[�ϊ��s����X�V

	// 
	void LookAt(DirectX::SimpleMath::Vector3 _targetPos);
};


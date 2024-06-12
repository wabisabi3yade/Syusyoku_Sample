#pragma once
#include "GameObject.h"

// �Q�[�����J�����N���X
class Camera : public GameObject
{
	// �J�����̒����_
	DirectX::SimpleMath::Vector3 focusPos{ 0,0,0 };
	// �J�����̏�x�N�g��
	DirectX::SimpleMath::Vector3 camUp{ 0,1.f,0 };
public:
	Camera();
	~Camera();

	void LateUpdate() override;

	void UpdateViewMatrix();	// �r���[�ϊ��s����X�V
};


#pragma once
#include "GameObject.h"

// �Q�[�����J�����N���X
class Camera : public GameObject
{
	// �J�����̒����_
	DirectX::SimpleMath::Vector3 focusPos{ 0,0,0 };
	// �J�����̏�x�N�g��
	DirectX::SimpleMath::Vector3 camUp{ 0,1.f,0 };

	float fov;	// ����p
	// �`�拗��
	float nearZ;	// �ŒZ
	float farZ;	 // �Œ�
	bool isOrthographic;	// ���s���e�����邩
	void UpdatePerspective(u_int _viewPortSlot = 0);	// �������e���Z�b�g����
	void UpdateOrthographic(u_int _viewPortSlot = 0);	// �������e���Z�b�g����
public:
	Camera();
	~Camera();

	void UpdateViewMatrix();	// �r���[�ϊ��s����X�V
	
	// �e�v���W�F�N�V�����ɐݒ�
	void SetOrthographic();	// ���s���e
	void SetPerspective();	// �������e

	void SetFocusPos(const DirectX::SimpleMath::Vector3& _focusPos);
};


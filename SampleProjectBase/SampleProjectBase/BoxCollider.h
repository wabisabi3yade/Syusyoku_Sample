#pragma once
#include "Collider.h"

class BoxCollider :
	public /*Collider*/ Component
{
	DirectX::SimpleMath::Vector3 offset; // �Q�[���I�u�W�F�N�g����̍��W�I�t�Z�b�g
	DirectX::SimpleMath::Vector3 angle;    // �X��
	DirectX::SimpleMath::Vector3 size;  // �{�b�N�X�̑傫��

	bool isHit = false;	// ����
public:
	using Component::Component;

	void Init();
	void Update() override;
	void Draw() override;

	void SetParameter() override;

	// �p�����[�^�ݒ�
	// ���W�I�t�Z�b�g
	void SetOffset(const DirectX::SimpleMath::Vector3& _offset) { offset = _offset; } 
	void SetAngle(const DirectX::SimpleMath::Vector3& _angle) { angle = _angle; }    // �p�x
	void SetSize(const DirectX::SimpleMath::Vector3& _size) { size = _size; } // �傫��
};


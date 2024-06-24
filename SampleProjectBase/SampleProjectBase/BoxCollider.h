#pragma once
#include "Collider.h"

class BoxCollider :
	public Collider
{
	DirectX::SimpleMath::Vector3 posOffset; // �Q�[���I�u�W�F�N�g����̍��W�I�t�Z�b�g
	DirectX::SimpleMath::Vector3 angleOffset;    // �X��
	DirectX::SimpleMath::Vector3 size;  // �{�b�N�X�̑傫��

	bool isAABB = false;	// AABB�Ȃ̂��t���O
public:
	using Collider::Collider;

	void Init();
	void Draw() override;

	void SetParameter() override;

	// �p�����[�^�ݒ�
	// ���W�I�t�Z�b�g
	void SetOffset(const DirectX::SimpleMath::Vector3& _offset) { posOffset = _offset; } 
	void SetAngle(const DirectX::SimpleMath::Vector3& _angle) { angleOffset = _angle; }    // �p�x
	void SetSize(const DirectX::SimpleMath::Vector3& _size) { size = _size; } // �傫��

	// �����蔻��Ŏg�p����
	// ���[���h���W�n�̊e�p�����[�^���擾����
	DirectX::SimpleMath::Vector3 GetCenterPos()const;
	DirectX::SimpleMath::Vector3 GetRotation()const;
	DirectX::SimpleMath::Vector3 GetScale()const;
};


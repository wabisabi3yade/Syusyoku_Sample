#pragma once

// ���W�E��]�E�X�P�[�����܂Ƃ߂��p�����[�^
class Transform
{
	// �e�����̒P�ʃx�N�g��
	DirectX::SimpleMath::Vector3 up;	// ��
	DirectX::SimpleMath::Vector3 right;	// �E
	DirectX::SimpleMath::Vector3 forward;	// �O
public:
	DirectX::SimpleMath::Vector3 position;	// ���W
	DirectX::SimpleMath::Vector3 rotation;	// ��]�p�x
	DirectX::SimpleMath::Vector3 scale;	// �X�P�[��
	DirectX::SimpleMath::Quaternion quaternion;	// �N�H�[�^�j�I��

	Transform() : scale(DirectX::SimpleMath::Vector3::One) {};
	~Transform() {};

	void UpdateVector();	// �����x�N�g�����X�V����

	void ResetParameter();	// �p�����[�^��������
	
	void LookAt(DirectX::SimpleMath::Vector3 _worldPos, DirectX::SimpleMath::Vector3 _upVector
	= DirectX::SimpleMath::Vector3::Up);	// ���̍��W�֌���

	const DirectX::SimpleMath::Vector3 Right()const { return right; }	// �E�x�N�g��
	const DirectX::SimpleMath::Vector3 Up()const { return up; }	// ��x�N�g��
	const DirectX::SimpleMath::Vector3 Forward()const { return forward; }	// ���ʃx�N�g��
};


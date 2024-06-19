#pragma once

// ���W�E��]�E�X�P�[�����܂Ƃ߂��p�����[�^
class Transform
{
	DirectX::SimpleMath::Vector3 forward;

public:
	DirectX::SimpleMath::Vector3 position;	// ���W
	DirectX::SimpleMath::Vector3 rotation;	// ��]�p�x
	DirectX::SimpleMath::Vector3 scale;	// �X�P�[��
	DirectX::SimpleMath::Quaternion quaternion;	// �N�H�[�^�j�I��


	Transform() : scale(DirectX::SimpleMath::Vector3::One) {};
	~Transform() {};

	void UpdateVector();	// �����x�N�g�����X�V����

	void ResetParameter();	// �p�����[�^��������

	DirectX::SimpleMath::Vector3 Forward()const { return forward; }	// ���ʃx�N�g�������߂�

	void LookAt(DirectX::SimpleMath::Vector3 _worldPos);
};


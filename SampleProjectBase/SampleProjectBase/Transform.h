#pragma once

// ���W�E��]�E�X�P�[�����܂Ƃ߂��p�����[�^
class Transform
{
public: 
	DirectX::SimpleMath::Vector3 position;	// ���W
	DirectX::SimpleMath::Vector3 rotation;	// ��]�p�x
	DirectX::SimpleMath::Vector3 scale;	// �X�P�[��
	DirectX::SimpleMath::Quaternion quaternion;	// �N�H�[�^�j�I��

	Transform() : scale(DirectX::SimpleMath::Vector3::One) {};
	~Transform() {};

	void ResetParameter();	// �p�����[�^��������
};


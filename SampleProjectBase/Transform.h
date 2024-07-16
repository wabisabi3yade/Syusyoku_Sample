#pragma once

// ���W�E��]�E�X�P�[�����܂Ƃ߂��p�����[�^
class Transform
{
	/// @brief �e�g�����X�t�H�[��
	Transform* pParent;

	// �e�����̒P�ʃx�N�g��
	DirectX::SimpleMath::Vector3 up;	// ��
	DirectX::SimpleMath::Vector3 right;	// �E
	DirectX::SimpleMath::Vector3 forward;	// �O
public:
	DirectX::SimpleMath::Vector3 position;	// ���W
	DirectX::SimpleMath::Vector3 rotation;	// ��]�p�x
	DirectX::SimpleMath::Vector3 scale;	// �X�P�[��
	DirectX::SimpleMath::Quaternion quaternion;	// �N�H�[�^�j�I��

	Transform() : scale(DirectX::SimpleMath::Vector3::One), pParent(nullptr) {};
	~Transform() {};

	void UpdateVector();	// �����x�N�g�����X�V����

	void ResetParameter();	// �p�����[�^��������
	
	// ���̍��W�ɃI�u�W�F�N�g��������
	void LookAt(DirectX::SimpleMath::Vector3 _worldPos, DirectX::SimpleMath::Vector3 _upVector
	= DirectX::SimpleMath::Vector3::Up);	

	/// @brief �e�g�����X�t�H�[����ݒ肷��
	/// @param _parent �e�g�����X�t�H�[���̎Q��
	void SetParent(Transform& _parent);

	const DirectX::SimpleMath::Vector3 Right()const { return right; }	// �E�x�N�g��
	const DirectX::SimpleMath::Vector3 Up()const { return up; }	// ��x�N�g��
	const DirectX::SimpleMath::Vector3 Forward()const { return forward; }	// ���ʃx�N�g��

	/// @brief ���ʃx�N�g�������炷(LookAt���Ɏg�p)
	void ShiftForward() { forward.x += 0.001f; }
};


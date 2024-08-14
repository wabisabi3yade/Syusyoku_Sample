#pragma once

// ���W�E��]�E�X�P�[�����܂Ƃ߂��p�����[�^
class Transform
{
	/// @brief �e�g�����X�t�H�[��
	Transform* pParent;

	/// @brief �q�g�����X�t�H�[��
	std::list<Transform> pChilds;

	// �e�����̒P�ʃx�N�g��
	DirectX::SimpleMath::Vector3 up;	// ��
	DirectX::SimpleMath::Vector3 right;	// �E
	DirectX::SimpleMath::Vector3 forward;	// �O

	DirectX::SimpleMath::Vector3 eularAngles;	// ��]�p�x
	DirectX::SimpleMath::Quaternion rotation;	// �N�H�[�^�j�I��
public:
	DirectX::SimpleMath::Vector3 position;	// ���W
	DirectX::SimpleMath::Vector3 scale;	// �X�P�[��
	

	Transform();
	~Transform() {};

	// �����x�N�g�����X�V����
	void UpdateVector();	

	// �p�����[�^��������
	void ResetParameter();	
	
	// ���̍��W�ɃI�u�W�F�N�g��������
	void LookAt(DirectX::SimpleMath::Vector3 _worldPos, DirectX::SimpleMath::Vector3 _upVector
	= DirectX::SimpleMath::Vector3::Up);	

	/// @brief �e�g�����X�t�H�[����ݒ肷��
	/// @param _parent �e�g�����X�t�H�[���̎Q��
	void SetParent(Transform& _parent);

	// �I�C���[�p���Z�b�g
	void SetEularAngleX(float _angle_x);	// x��
	void SetEularAngleY(float _angle_y);	// y��
	void SetEularAngleZ(float _angle_z);	// z��
	void SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles);

	// �N�H�[�^�j�I�����Z�b�g
	void SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion);

	// �I�C���[�p���擾
	DirectX::SimpleMath::Vector3 GetEularAngles() const;

	// �N�H�[�^�j�I�����擾
	DirectX::SimpleMath::Quaternion GetRotation() const;

	DirectX::SimpleMath::Vector3 Right()const { return right; }	// �E�x�N�g��
	DirectX::SimpleMath::Vector3 Up()const { return up; }	// ��x�N�g��
	DirectX::SimpleMath::Vector3 Forward()const { return forward; }	// ���ʃx�N�g��
};


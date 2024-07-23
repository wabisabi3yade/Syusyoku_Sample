#pragma once

// �E�F�C�g�̍ő吔
constexpr u_int MAX_WEIGHT_NUM(4);

// ���_�f�[�^
struct Vertex
{
	/// @brief ���W
	DirectX::SimpleMath::Vector3 position;

	// �F(RGBA)
	DirectX::SimpleMath::Color color;

	/// @brief UV���W�i�e�N�X�`�����W�j
	DirectX::SimpleMath::Vector2 uv;

	/// @brief ���_�̖@���x�N�g��
	DirectX::SimpleMath::Vector3 normal;

	/// @brief �e�������{�[����ID
	int boneIndex[MAX_WEIGHT_NUM]{ 0 };

	/// @brief �{�[���̉e���x
	float boneWeight[MAX_WEIGHT_NUM]{ 0.0f };

	/// @brief �{�[���̐�
	u_int boneCnt{ 0 };
};


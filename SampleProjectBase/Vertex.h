#pragma once

namespace HashiTaku
{
	// �E�F�C�g�̍ő吔
	constexpr u_int MAX_WEIGHT_NUM(4);

	// ���_�f�[�^
	struct Vertex
	{
		/// @brief ���W
		DXSimp::Vector3 position;

		// �F(RGBA)
		DXSimp::Color color;

		/// @brief UV���W�i�e�N�X�`�����W�j
		DXSimp::Vector2 uv;

		/// @brief ���_�̖@���x�N�g��
		DXSimp::Vector3 normal;

		/// @brief �e�������{�[����ID
		int boneIndex[MAX_WEIGHT_NUM]{ -1,-1,-1,-1 };

		/// @brief �{�[���̉e���x
		float boneWeight[MAX_WEIGHT_NUM]{ 0.0f };
	};
}
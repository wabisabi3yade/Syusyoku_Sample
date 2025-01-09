#include "pch.h"
#include "Polygon2D.h"

namespace HashiTaku
{
	constexpr short VERTEX_NUM(4);	// ���_��
	constexpr short INDEX_NUM(4);	// �C���f�b�N�X��
	constexpr short LEFT_UP_IDX(0);	// ����̃C���f�b�N�X
	constexpr short RIGHT_UP_IDX(1);	// �E��̃C���f�b�N�X
	constexpr short LEFT_DOWN_IDX(2);	// �����̃C���f�b�N�X
	constexpr short RIGHT_DOWN_IDX(3);	// �E���̃C���f�b�N�X

	Polygon2D::Polygon2D()
	{
		using namespace DXSimp;

		verticies.resize(VERTEX_NUM);
		verticies[LEFT_UP_IDX].uv = { 0.0f, 0.0f };
		verticies[RIGHT_UP_IDX].uv = { 1.0f, 0.0f };
		verticies[LEFT_DOWN_IDX].uv = { 0.0f, 1.0f };
		verticies[RIGHT_DOWN_IDX].uv = { 1.0f, 1.0f };

		verticies[LEFT_UP_IDX].color = Color(1.0f, 1.0f, 1.0f);
		verticies[RIGHT_UP_IDX].color = Color(1.0f, 1.0f, 1.0f);
		verticies[LEFT_DOWN_IDX].color = Color(1.0f, 1.0f, 1.0f);
		verticies[RIGHT_DOWN_IDX].color = Color(1.0f, 1.0f, 1.0f);

		indicies.resize(INDEX_NUM);
		indicies[LEFT_UP_IDX] = LEFT_UP_IDX;
		indicies[RIGHT_UP_IDX] = RIGHT_UP_IDX;
		indicies[LEFT_DOWN_IDX] = LEFT_DOWN_IDX;
		indicies[RIGHT_DOWN_IDX] = RIGHT_DOWN_IDX;

		// �쐬
		MakePolygon(Vector3::Zero, Vector2::One, Vector3::Zero);

		// �����ݒ�
		InitSetting("Polygon2D", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	void Polygon2D::MakePolygon(const DXSimp::Vector3& _centerPos, const DXSimp::Vector2& _size, const DXSimp::Vector3& _angles)
	{
		using namespace DXSimp;

		// ���_�쐬
		Vector2 halfSize = _size * 0.5f;
		Matrix rotateMtx = Mtx::CreateRoratateMtx(_angles);

		// ���W�A�F�A���_
		for (int i = 0; i < VERTEX_NUM; i++)
		{
			// �|���S���̒��_�x�N�g��
			short vecX = 1;
			short vecY = 1;
			if (i == LEFT_UP_IDX || i == LEFT_DOWN_IDX)
				vecX = -1;
			if (i == LEFT_UP_IDX || i == RIGHT_UP_IDX)
				vecY = -1;

			// �|���S���̍��W�����߂�
			Vector3 vertexPos = { vecX * halfSize.x, vecY * halfSize.y, 0.0f };

			// ���W�Ɋp�x�𔽉f
			vertexPos = Vector3::Transform(vertexPos, rotateMtx);

			// �ړ�
			verticies[i].position = vertexPos + _centerPos;
		}

		// �o�b�t�@�쐬
		InitBuffer();
	}

	void Polygon2D::SetUV(const DXSimp::Vector2& _startUV, const DXSimp::Vector2& _endUV)
	{
		verticies[LEFT_UP_IDX].uv = _startUV;	// ����
		verticies[RIGHT_UP_IDX].uv = { _endUV.x, _startUV.y };	//�@�E��
		verticies[LEFT_DOWN_IDX].uv = { _startUV.x, _endUV.y };	// �E��
		verticies[RIGHT_DOWN_IDX].uv = _endUV;	// ����

		InitBuffer();
	}
}
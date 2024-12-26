#include "pch.h"
#include "Polygon2D.h"

namespace HashiTaku
{
	constexpr short VERTEX_NUM(4);	// 頂点数
	constexpr short INDEX_NUM(4);	// インデックス数
	constexpr short LEFT_UP_IDX(0);	// 左上のインデックス
	constexpr short RIGHT_UP_IDX(1);	// 右上のインデックス
	constexpr short LEFT_DOWN_IDX(2);	// 左下のインデックス
	constexpr short RIGHT_DOWN_IDX(3);	// 右下のインデックス

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

		// 作成
		MakePolygon(Vector3::Zero, Vector2::One, Vector3::Zero);

		// 初期設定
		InitSetting("Polygon2D", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	void Polygon2D::MakePolygon(const DXSimp::Vector3& _centerPos, const DXSimp::Vector2& _size, const DXSimp::Vector3& _angles)
	{
		using namespace DXSimp;

		// 頂点作成
		Vector2 halfSize = _size * 0.5f;
		Matrix rotateMtx = Mtx::CreateRoratateMtx(_angles);

		// 座標、色、頂点
		for (int i = 0; i < VERTEX_NUM; i++)
		{
			// ポリゴンの頂点ベクトル
			short vecX = 1;
			short vecY = 1;
			if (i == LEFT_UP_IDX || i == LEFT_DOWN_IDX)
				vecX = -1;
			if (i == LEFT_UP_IDX || i == RIGHT_UP_IDX)
				vecY = -1;

			// ポリゴンの座標を求める
			Vector3 vertexPos = { vecX * halfSize.x, vecY * halfSize.y, 0.0f };

			// 座標に角度を反映
			vertexPos = Vector3::Transform(vertexPos, rotateMtx);

			// 移動
			verticies[i].position = vertexPos + _centerPos;
		}

		// バッファ作成
		InitBuffer();
	}

	void Polygon2D::SetUV(const DXSimp::Vector2& _startUV, const DXSimp::Vector2& _endUV)
	{
		verticies[LEFT_UP_IDX].uv = _startUV;	// 左上
		verticies[RIGHT_UP_IDX].uv = { _endUV.x, _startUV.y };	//　右上
		verticies[LEFT_DOWN_IDX].uv = { _startUV.x, _endUV.y };	// 右下
		verticies[RIGHT_DOWN_IDX].uv = _endUV;	// 左下

		InitBuffer();
	}
}
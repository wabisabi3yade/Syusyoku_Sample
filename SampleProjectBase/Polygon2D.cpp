#include "pch.h"
#include "Polygon2D.h"

constexpr short VERTEX_NUM(4);	// 頂点数
constexpr short INDEX_NUM(4);	// インデックス数
constexpr short LEFT_UP_IDX(0);	// 左上のインデックス
constexpr short RIGHT_UP_IDX(1);	// 右上のインデックス
constexpr short LEFT_DOWN_IDX(2);	// 左下のインデックス
constexpr short RIGHT_DOWN_IDX(3);	// 右下のインデックス

Polygon2D::Polygon2D()
{
	using namespace DirectX::SimpleMath;

	// 作成
	MakePolygon(Vector2::Zero, Vector2::One, Vector3::Zero);
}

void Polygon2D::MakePolygon(const DirectX::SimpleMath::Vector2& _centerPos, const DirectX::SimpleMath::Vector2& _size, const DirectX::SimpleMath::Vector3& _angles)
{
	using namespace DirectX::SimpleMath;

	// 頂点作成
	verticies.resize(VERTEX_NUM);

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
		if (i == LEFT_DOWN_IDX || i == RIGHT_DOWN_IDX)
			vecY = -1;

		// ポリゴンの座標を求める
		Vector3 vertexPos = { vecX * halfSize.x, vecY * halfSize.y, 0.0f };

		// 座標に角度を反映
		vertexPos = Vector3::Transform(vertexPos, rotateMtx);

		// 移動
		verticies[i].position = vertexPos + _centerPos;

		verticies[i].color = Vector4::One;	// 色
		verticies[i].normal = Vector3(0.0f, 1.0f, 0.0f);
	}

	// uv座標
	verticies[LEFT_UP_IDX].uv = { 0.0f, 0.0f };
	verticies[RIGHT_UP_IDX].uv = { 1.0f, 0.0f };
	verticies[LEFT_DOWN_IDX].uv = { 0.0f, 1.0f };
	verticies[RIGHT_DOWN_IDX].uv = { 1.0f, 1.0f };

	// インデックス
	indicies.resize(INDEX_NUM);

	indicies[LEFT_UP_IDX] = LEFT_UP_IDX;
	indicies[RIGHT_UP_IDX] = RIGHT_UP_IDX;
	indicies[LEFT_DOWN_IDX] = LEFT_DOWN_IDX;
	indicies[RIGHT_DOWN_IDX] = RIGHT_DOWN_IDX;
	// バッファ作成
	InitBuffer();

	// 初期設定
	InitSetting("Polygon2D", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Polygon2D::SetUV(const DirectX::SimpleMath::Vector2& _startUV, const DirectX::SimpleMath::Vector2& _endUV)
{
	verticies[0].uv = _startUV;	// 左上
	verticies[1].uv = { _endUV.x, _startUV.y };	//　右上
	verticies[2].uv = _endUV;	// 右下
	verticies[3].uv = { _startUV.x, _endUV.y };	// 左下
}
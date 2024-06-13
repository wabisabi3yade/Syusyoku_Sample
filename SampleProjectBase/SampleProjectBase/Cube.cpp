#include "pch.h"
#include "Cube.h"

constexpr short FACE_NUM = (12);	// 三角形ポリゴン数
constexpr short VERTEX_NUM = (8);	// 頂点数
constexpr short INDEX_NUM = (24);	// 頂点インデックス数

using namespace DirectX::SimpleMath;

Cube::Cube() : length(Vector3::One)
{
	Make();
}

Cube::~Cube()
{
}

void Cube::Draw()
{
	// トポロジーを設定
	Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	BasicObject_Base::Draw();

	length = Vector3::One;	// 長さを戻す	
}

void Cube::Make()
{

	// キューブの頂点座標作成
	vertexNum = VERTEX_NUM;
	pVertices = new Vertex[vertexNum];
	pVertices[0].position.x = -(length.x / 2.0);
	pVertices[0].position.y = (length.y / 2.0f);
	pVertices[0].position.z = -(length.z / 2.0f);

	pVertices[1].position.x = (length.x / 2.0f);
	pVertices[1].position.y = ( length.y / 2.0f );
	pVertices[1].position.z = -(length.z / 2.0f);

	pVertices[2].position.x = ( length.x / 2.0f );
	pVertices[2].position.y = -( length.y / 2.0f );
	pVertices[2].position.z = -(length.z / 2.0f);

	pVertices[3].position.x = -(length.x / 2.0f);
	pVertices[3].position.y = -(length.y / 2.0f);
	pVertices[3].position.z = -(length.z / 2.0f);

	pVertices[4].position.x = -(length.x / 2.0f);
	pVertices[4].position.y = (length.y / 2.0f);
	pVertices[4].position.z = (length.z / 2.0f);

	pVertices[5].position.x = (length.x / 2.0f);
	pVertices[5].position.y = (length.y / 2.0f);
	pVertices[5].position.z = (length.z / 2.0f);

	pVertices[6].position.x = (length.x / 2.0f);
	pVertices[6].position.y = -(length.y / 2.0f);
	pVertices[6].position.z = (length.z / 2.0f);

	pVertices[7].position.x = -(length.x / 2.0f);
	pVertices[7].position.y = -(length.y / 2.0f);
	pVertices[7].position.z = (length.z / 2.0f);

	// インデックスを設定(TRIANGLESTRIP)
	indexNum = INDEX_NUM;
	pIndicies = new u_int[indexNum];

	// 手前
	pIndicies[0] = 0;
	pIndicies[1] = 1;
	pIndicies[2] = 3;
	pIndicies[3] = 2;

	// 奥
	pIndicies[4] = 5;
	pIndicies[5] = 4;
	pIndicies[6] = 6;
	pIndicies[7] = 7;

	// 右
	pIndicies[8] = 1;
	pIndicies[9] = 5;
	pIndicies[10] = 2;
	pIndicies[11] = 6;

	// 左
	pIndicies[12] = 4;
	pIndicies[13] = 0;
	pIndicies[14] = 7;
	pIndicies[15] = 3;

	// 上
	pIndicies[16] = 4;
	pIndicies[17] = 5;
	pIndicies[18] = 0;
	pIndicies[19] = 1;

	// 下
	pIndicies[20] = 3;
	pIndicies[21] = 2;
	pIndicies[22] = 7;
	pIndicies[23] = 6;

	// 全頂点に
	for (short i = 0; i < VERTEX_NUM; i++)
	{
		pVertices[i].color = color;	// 頂点カラー		
	}

	// バッファ作成
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	CreateVertexBuffer(*renderer);
	CreateIndexBuffer(*renderer);
}
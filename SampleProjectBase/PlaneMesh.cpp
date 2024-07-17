#include "pch.h"
#include "PlaneMesh.h"

constexpr u_int VERTEX_NUM(4);
constexpr u_int INDEX_NUM(4);

using namespace DirectX::SimpleMath;

void PlaneMesh::MakePlane()
{
	// 頂点作成
	verticies.resize(VERTEX_NUM);

	// 座標
	verticies[0].position = Vector3(-0.5f, 0.0f, 0.5f);	// 左上
	verticies[1].position = Vector3(0.5f, 0.0f, 0.5f);	// 右上
	verticies[2].position = Vector3(-0.5f, 0.0f, -0.5f);		// 左下
	verticies[3].position = Vector3(0.5f,0.0f, -0.5f);	// 右下

	// uv座標
	verticies[0].uv = { 0.0f, 0.0f };
	verticies[1].uv = { 1.0f, 0.0f };
	verticies[2].uv = { 0.0f, 1.0f };
	verticies[3].uv = { 1.0f, 1.0f };

	// 色、頂点
	for (int i = 0; i < VERTEX_NUM; i++)
	{
		verticies[i].color = Vector4::One;	// 色
		verticies[i].normal = Vector3(0.0f, 0.0f, -1.0f);
	}

	// インデックス
	indicies.resize(INDEX_NUM);

	indicies[0] = 0;
	indicies[1] = 1;
	indicies[2] = 2;
	indicies[3] = 3;
	// バッファ作成
	InitBuffer();

	// 初期設定
	InitSetting("Mesh_Plane", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

PlaneMesh::PlaneMesh()
{
	MakePlane();
} 

void PlaneMesh::SetUV(const DirectX::SimpleMath::Vector2& _startUV, const DirectX::SimpleMath::Vector2& _endUV)
{
	verticies[0].uv = _startUV;	// 左上
	verticies[1].uv = { _endUV.x, _startUV.y };	//　右上
	verticies[2].uv = _endUV;	// 右下
	verticies[3].uv = { _startUV.x, _endUV.y };	// 左下
}

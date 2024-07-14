#include "pch.h"
#include "PlaneMesh.h"

constexpr u_int VERTEX_NUM(3);
constexpr u_int INDEX_NUM(3);

using namespace DirectX::SimpleMath;

void PlaneMesh::MakePlane()
{
	// 頂点作成
	verticies.resize(VERTEX_NUM);

	// 座標
	verticies[0].position = Vector3(-0.5f, -0.5f, 0);	// 左上
	verticies[1].position = Vector3(0.5f, -0.5f, 0);	// 右上
	verticies[2].position = Vector3(0.5f, 0.5f, 0);		// 右下
	//verticies[3].position = Vector3(-0.5f, 0.5f, 0);	// 左下

	// uv座標
	verticies[0].uv = { 0.0f, 0.0f };
	verticies[1].uv = { 1.0f, 0.0f };
	verticies[2].uv = { 1.0f, 1.0f };
	/*verticies[3].uv = { 0.0f, 1.0f };*/

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
	/*indicies[3] = 0;
	indicies[4] = 2;
	indicies[5] = 3;*/

	// バッファ作成
	InitBuffer();

	// 初期設定
	InitSetting("Mesh_Plane", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

PlaneMesh::PlaneMesh()
{
	MakePlane();
}

PlaneMesh::PlaneMesh(const PlaneMesh& _other)
{
	Copy(_other);
}

PlaneMesh& PlaneMesh::operator=(const PlaneMesh& _other)
{
	Copy(_other);

	return *this;
}

void PlaneMesh::SetUV(const DirectX::SimpleMath::Vector2& _startUV, const DirectX::SimpleMath::Vector2& _endUV)
{
	verticies[0].uv = _startUV;
	//verticies[1].uv = 

}

void PlaneMesh::Copy(const PlaneMesh& _other)
{
	if (this == &_other) return;

	SingleMesh::Copy(_other);
}

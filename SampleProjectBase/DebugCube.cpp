#include "pch.h"
#include "DebugCube.h"

constexpr u_int VERTEX_NUM(8);
constexpr u_int INDEX_NUM(24);

using namespace DirectX::SimpleMath;

void DebugCube::Make()
{
	const float d = 0.5f;

	DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT3 normal[] = {
		DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f,-1.0f),
	};

	// 頂点データを作成
	verticies.resize(VERTEX_NUM);
	verticies[0] = { DirectX::XMFLOAT3(-d, d,-d), color, Vector2::One,normal[1] };
	verticies[1] = { DirectX::XMFLOAT3(d, d, -d), color, Vector2::One,normal[0] };
	verticies[2] = { DirectX::XMFLOAT3(-d, -d, -d), color, Vector2::One,normal[3] };
	verticies[3] = { DirectX::XMFLOAT3(d, -d, -d), color, Vector2::One,normal[3] };
	verticies[4] = { DirectX::XMFLOAT3(-d, d, d), color, Vector2::One,normal[2] };
	verticies[5] = { DirectX::XMFLOAT3(d, d, d), color, Vector2::One,normal[0] };
	verticies[6] = { DirectX::XMFLOAT3(-d, -d, d), color, Vector2::One,normal[1] };
	verticies[7] = { DirectX::XMFLOAT3(d, -d, d), color, Vector2::One,normal[4] };

	// インデックスデータを作成
	indicies.resize(INDEX_NUM);
	u_int idx[] =
	{
		0,1, 0,2, 2,3,
		1,3, 1,5, 5,7,
		3,7, 6,7, 2,6,
		4,6, 0,4, 4,5,
	};

	for (u_int index = 0; index < INDEX_NUM; index++)
	{
		indicies[index] = idx[index];
	}

	InitSetting("Mesh_WireCube", D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	InitBuffer();
}

DebugCube::DebugCube()
{
	Make();
}
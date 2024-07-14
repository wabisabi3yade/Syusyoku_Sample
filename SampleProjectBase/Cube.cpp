#include "pch.h"
#include "Cube.h"

constexpr short VERTEX_NUM = (24);	// 頂点数
constexpr short INDEX_NUM = (36);	// 頂点インデックス数

using namespace DirectX::SimpleMath;

Cube::Cube()
{
	Make();
}

void Cube::Make()
{
	std::unique_ptr<SingleMesh> pMesh = std::make_unique<SingleMesh>();

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
	verticies[0] = { DirectX::XMFLOAT3(-d, d,-d), color, Vector2::One,normal[0] };
	verticies[1] = { DirectX::XMFLOAT3(d, d, -d), color, Vector2::One,normal[0] };
	verticies[2] = { DirectX::XMFLOAT3(-d,-d,-d), color, Vector2::One,normal[0] };
	verticies[3] = { DirectX::XMFLOAT3(d,-d, -d), color, Vector2::One,normal[0] };

	verticies[4] = { DirectX::XMFLOAT3(-d, d, d), color, Vector2::One,normal[1] };
	verticies[5] = { DirectX::XMFLOAT3(-d, -d,d), color,Vector2::One ,normal[1] };
	verticies[6] = { DirectX::XMFLOAT3(d,d, d), color, Vector2::One,normal[1] };
	verticies[7] = { DirectX::XMFLOAT3(d,-d,d), color, Vector2::One,normal[1] };

	verticies[8] = { DirectX::XMFLOAT3(-d, d, d), color,Vector2::One,normal[2] };
	verticies[9] = { DirectX::XMFLOAT3(-d, d, -d), color, Vector2::One,normal[2] };
	verticies[10] = { DirectX::XMFLOAT3(-d, -d,d), color,Vector2::One ,normal[2] };
	verticies[11] = { DirectX::XMFLOAT3(-d, -d,-d), color, Vector2::One,normal[2] };
	
	verticies[12] = { DirectX::XMFLOAT3(d,d,d), color,Vector2::One ,normal[3] };
	verticies[13] = { DirectX::XMFLOAT3(d,-d,d), color, Vector2::One,normal[3] };
	verticies[14] = { DirectX::XMFLOAT3(d,d, -d), color,Vector2::One ,normal[3] };
	verticies[15] = { DirectX::XMFLOAT3(d,-d, -d), color,Vector2::One,normal[3] };
	
	verticies[16] = { DirectX::XMFLOAT3(-d, d, d), color, Vector2::One,normal[4] };
	verticies[17] = { DirectX::XMFLOAT3(d, d, d), color,Vector2::One ,normal[4] };
	verticies[18] = { DirectX::XMFLOAT3(-d,d, -d), color, Vector2::One,normal[4] };
	verticies[19] = { DirectX::XMFLOAT3(d,d, -d), color,Vector2::One,normal[4] };
	
	verticies[20] = { DirectX::XMFLOAT3(-d,-d,d), color,Vector2::One ,normal[5] };
	verticies[21] = { DirectX::XMFLOAT3(-d,-d,-d), color, Vector2::One,normal[5] };
	verticies[22] = { DirectX::XMFLOAT3(d, -d,d), color,Vector2::One ,normal[5] };
	verticies[23] = { DirectX::XMFLOAT3(d, -d,-d), color,Vector2::One,normal[5] };


	// インデックスデータ作成
	indicies.resize(INDEX_NUM);

	u_int idx[] = {
		 0, 1, 2,  3, 2, 1,
		 4, 5, 6,  7, 6, 5,
		8, 9, 10,  11,10,9,
		12,13,14, 15,14,13,
		16,17,18, 19,18,17,
		20,21,22, 23,22,21,
	};

	for (u_int i = 0; i < INDEX_NUM; i++)
	{
		indicies[i] = idx[i];
	}

	InitBuffer();

	InitSetting("Mesh_Cube", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
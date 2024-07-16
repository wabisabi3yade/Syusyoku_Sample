#include "pch.h"
#include "Sphere.h"

using namespace DirectX::SimpleMath;
void Sphere::Make()
{
	// 頂点データ作成
	const int detail = 32;
	for (int j = 0; j < detail / 2 + 1; ++j)
	{
		float radY = DirectX::XM_2PI / detail * j;
		for (int i = 0; i < detail; ++i)
		{
			float radXZ = DirectX::XM_2PI / detail * i;
			float x = sinf(radY) * sinf(radXZ);
			float y = cosf(radY);
			float z = sinf(radY) * cosf(radXZ);
			Vertex vertex;
			vertex.position = Vector3(x * 0.5f, y * 0.5f, z * 0.5f);
			vertex.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
			vertex.normal = vertex.position;
			vertex.normal.Normalize();
			vertex.uv = Vector2::Zero;
			
			verticies.push_back(vertex);
		}
	}

	// インデックスデータ作成
	std::vector<u_int> indicies;
	for (int j = 0; j < detail / 2; ++j)
	{
		for (int i = 0; i < detail; ++i)
		{
			indicies.push_back((j + 1) * detail + i);
			indicies.push_back(j * detail + i);
			indicies.push_back(j * detail + ((i + 1) % detail));
			indicies.push_back(indicies.back());
			indicies.push_back((j + 1) * detail + ((i + 1) % detail));
			indicies.push_back((j + 1) * detail + i);
		}
	}

	InitSetting("Mesh_Sphere", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	InitBuffer();
}

Sphere::Sphere()
{
	Make();
}
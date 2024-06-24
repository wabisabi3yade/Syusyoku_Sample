#include "pch.h"
#include "Cube.h"

constexpr short VERTEX_NUM = (24);	// 頂点数
constexpr short INDEX_NUM = (36);	// 頂点インデックス数

using namespace DirectX::SimpleMath;

Cube::Cube()
{
	Make();
}

Cube::~Cube()
{
}

void Cube::Draw(Transform& _transform, DirectX::SimpleMath::Color& _color)
{
	// トポロジーを設定
	ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	BasicObject_Base::Draw(_transform, _color);
}

void Cube::Make()
{
	vertexNum = VERTEX_NUM;
	pVertices = new Vertex[vertexNum];
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

	pVertices[0] = { DirectX::XMFLOAT3(-d, d,-d), color, Vector2::One,normal[0] };
	pVertices[1] = { DirectX::XMFLOAT3(d, d, -d), color, Vector2::One,normal[0] };
	pVertices[2] = { DirectX::XMFLOAT3(-d,-d,-d), color, Vector2::One,normal[0] };
	pVertices[3] = { DirectX::XMFLOAT3(d,-d, -d), color, Vector2::One,normal[0] };

	pVertices[4] = { DirectX::XMFLOAT3(-d, d, d), color, Vector2::One,normal[1] };
	pVertices[5] = { DirectX::XMFLOAT3(-d, -d,d), color,Vector2::One ,normal[1] };
	pVertices[6] = { DirectX::XMFLOAT3(d,d, d), color, Vector2::One,normal[1] };
	pVertices[7] = { DirectX::XMFLOAT3(d,-d,d), color, Vector2::One,normal[1] };

	pVertices[8] = { DirectX::XMFLOAT3(-d, d, d), color,Vector2::One,normal[2] };
	pVertices[9] = { DirectX::XMFLOAT3(-d, d, -d), color, Vector2::One,normal[2] };
	pVertices[10] = { DirectX::XMFLOAT3(-d, -d,d), color,Vector2::One ,normal[2] };
	pVertices[11] = { DirectX::XMFLOAT3(-d, -d,-d), color, Vector2::One,normal[2] };
	
	pVertices[12] = { DirectX::XMFLOAT3(d,d,d), color,Vector2::One ,normal[3] };
	pVertices[13] = { DirectX::XMFLOAT3(d,-d,d), color, Vector2::One,normal[3] };
	pVertices[14] = { DirectX::XMFLOAT3(d,d, -d), color,Vector2::One ,normal[3] };
	pVertices[15] = { DirectX::XMFLOAT3(d,-d, -d), color,Vector2::One,normal[3] };
	
	pVertices[16] = { DirectX::XMFLOAT3(-d, d, d), color, Vector2::One,normal[4] };
	pVertices[17] = { DirectX::XMFLOAT3(d, d, d), color,Vector2::One ,normal[4] };
	pVertices[18] = { DirectX::XMFLOAT3(-d,d, -d), color, Vector2::One,normal[4] };
	pVertices[19] = { DirectX::XMFLOAT3(d,d, -d), color,Vector2::One,normal[4] };
	
	pVertices[20] = { DirectX::XMFLOAT3(-d,-d,d), color,Vector2::One ,normal[5] };
	pVertices[21] = { DirectX::XMFLOAT3(-d,-d,-d), color, Vector2::One,normal[5] };
	pVertices[22] = { DirectX::XMFLOAT3(d, -d,d), color,Vector2::One ,normal[5] };
	pVertices[23] = { DirectX::XMFLOAT3(d, -d,-d), color,Vector2::One,normal[5] };

	indexNum = INDEX_NUM;
	pIndicies = new u_int[indexNum];

	WORD idx[] = {
		 0, 1, 2,  3, 2, 1,
		 4, 5, 6,  7, 6, 5,
		8, 9, 10,  11,10,9,
		12,13,14, 15,14,13,
		16,17,18, 19,18,17,
		20,21,22, 23,22,21,
	};

	for (u_int i = 0; i < indexNum; i++)
	{
		pIndicies[i] = idx[i];
	}

	// バッファ作成
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	CreateVertexBuffer(*renderer);
	CreateIndexBuffer(*renderer);
}
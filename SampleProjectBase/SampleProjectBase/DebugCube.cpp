#include "pch.h"
#include "DebugCube.h"

using namespace DirectX::SimpleMath;

void DebugCube::Make()
{
	constexpr int VERTEX_NUM(8);
	constexpr int INDEX_NUM(24);

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

	pVertices[0] = { DirectX::XMFLOAT3(-d, d,-d), color, Vector2::One,normal[1] };
	pVertices[1] = { DirectX::XMFLOAT3(d, d, -d), color, Vector2::One,normal[0] };
	pVertices[2] = { DirectX::XMFLOAT3(-d, -d, -d), color, Vector2::One,normal[3] };
	pVertices[3] = { DirectX::XMFLOAT3(d, -d, -d), color, Vector2::One,normal[3] };

	pVertices[4] = { DirectX::XMFLOAT3(-d, d, d), color, Vector2::One,normal[2] };
	pVertices[5] = { DirectX::XMFLOAT3(d, d, d), color, Vector2::One,normal[0] };
	pVertices[6] = { DirectX::XMFLOAT3(-d, -d, d), color, Vector2::One,normal[1] };
	pVertices[7] = { DirectX::XMFLOAT3(d, -d, d), color, Vector2::One,normal[4] };


	indexNum = INDEX_NUM;
	pIndicies = new u_int[indexNum];

	WORD idx[] = 
	{
		0,1, 0,2, 2,3,
		1,3, 1,5, 5,7,
		3,7, 6,7, 2,6,
		4,6, 0,4, 4,5,
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

DebugCube::DebugCube()
{
	Make();
}

DebugCube::~DebugCube()
{
}

void DebugCube::Draw(Transform& _transform, DirectX::SimpleMath::Color& _color)
{
	// トポロジーを設定
	ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	BasicObject_Base::Draw(_transform, _color);
}

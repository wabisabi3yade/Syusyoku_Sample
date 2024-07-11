#include "pch.h"
#include "Sprite.h"

#include "Material.h"

constexpr u_int VERTEX_NUM(4);
constexpr u_int INDEX_NUM(6);

using namespace DirectX::SimpleMath;

Sprite::Sprite() : pTexture(nullptr)
{
	MakeSquare();
}

void Sprite::MakeSquare()
{
	// 四角ポリゴン作成
	pSquareMesh = std::make_unique<Mesh>();	

	// 頂点作成
	std::vector<Vertex>& verticies = pSquareMesh->GetVerticies();
	verticies.resize(VERTEX_NUM);

	// 座標
	verticies[0].position = Vector3(-0.5f, -0.5f, 0);	// 左上
	verticies[1].position = Vector3(0.5f, -0.5f, 0);	// 右上
	verticies[2].position = Vector3(0.5f, 0.5f, 0);	// 右下
	verticies[3].position = Vector3(-0.5f, 0.5f, 0);	// 左下

	// uv座標
	verticies[0].uv = { 0.0f, 0.0f };	
	verticies[1].uv = { 1.0, 0.0f };	
	verticies[2].uv = { 1.0f, 1.0f };	
	verticies[3].uv = { 0.0f, 1.0f };	

	// 色、頂点
	for (int i = 0; i < VERTEX_NUM; i++)
	{
		verticies[i].color = Vector4::One;	// 色
		verticies[i].normal = Vector3(0.0f, 0.0f, -1.0f);
	}

	// インデックス
	std::vector<u_int>& indicies = pSquareMesh->GetIndicies();
	indicies.resize(INDEX_NUM);

	indicies[0] = 0;
	indicies[1] = 1;
	indicies[2] = 2;
	indicies[3] = 0;
	indicies[4] = 2;
	indicies[5] = 3;

	// バッファ作成
	pSquareMesh->InitBuffer();
}


void Sprite::Copy(const Sprite& _other)
{
	// アドレスを渡す
	pTexture = _other.pTexture;

	// メッシュを作ってパラメータをコピー
	Mesh* pMesh = new Mesh(*_other.pSquareMesh);
	pSquareMesh.reset(pMesh);
}

Sprite::Sprite(const Sprite& _other)
{
	if (this == &_other) return;
	Copy(_other);
}

Sprite& Sprite::operator=(const Sprite& _other)
{
	if (this == &_other) return *this;
	Copy(_other);
	return *this;
}

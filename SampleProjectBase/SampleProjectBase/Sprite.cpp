#include "pch.h"
#include "Sprite.h"

#include "Material.h"

using namespace DirectX::SimpleMath;

Sprite::Sprite() : pTexture(nullptr)
{
	MakeSquare();
}

void Sprite::MakeSquare()
{
	// テクスチャをはるポリゴンを生成する(TRIANGLESTRIP)
	// 頂点座標
	constexpr int vertexNum = 4;
	Vertex* pVerticies = new Vertex[vertexNum];
	pVerticies[0].position = Vector3(-0.5f, -0.5f, 0);	// 左上
	pVerticies[1].position = Vector3(0.5f, -0.5f, 0);	// 右上
	pVerticies[2].position = Vector3(0.5f, 0.5f, 0);	// 右下
	pVerticies[3].position = Vector3(-0.5f, 0.5f, 0);	// 左下

	// uv座標
	pVerticies[0].uv = { 0.0f, 0.0f };	
	pVerticies[1].uv = { 1.0, 0.0f };	
	pVerticies[2].uv = { 1.0f, 1.0f };	
	pVerticies[3].uv = { 0.0f, 1.0f };	

	// インデックス
	constexpr int indexNum = 6;
	u_int* pIndicies = new u_int[indexNum];
	pIndicies[0] = 0;
	pIndicies[1] = 1;
	pIndicies[2] = 2;
	pIndicies[3] = 0;
	pIndicies[4] = 2;
	pIndicies[5] = 3;

	// 色、頂点
	for (int i = 0; i < vertexNum; i++)
	{
		pVerticies[i].color = Vector4::One;	// 色
		pVerticies[i].normal = Vector3(0.0f, 0.0f, -1.0f);
	}

	// 四角ポリゴン
	pSquareMesh = std::make_unique<Mesh>();	// メッシュ作成
	pSquareMesh->SetVertexData(pVerticies, vertexNum, pIndicies, indexNum);

}


void Sprite::Copy(const Sprite& _other)
{
	// アドレスを渡す
	pTexture = _other.pTexture;

	// メッシュを作ってパラメータをコピー
	pSquareMesh = std::make_unique<Mesh>();
}


void Sprite::Draw()
{
	pSquareMesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

Sprite::Sprite(const Sprite& _other)
{
	if (this == &_other) return;

	Copy(_other);
}

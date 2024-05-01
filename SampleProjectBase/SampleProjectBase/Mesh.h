#pragma once

struct aiMesh;

// メッシュクラス
// 頂点、辺（エッジ）、面（ポリゴン）の集合のこと
class Mesh
{

	Vertex* pVertices{ nullptr };	// メッシュの頂点データ

	u_int* pIndicies{ nullptr };

	u_int vertexNum;	// 頂点数

	u_int indexNum;	// 頂点インデックス

	ID3D11Buffer* pVertexBuffer{ nullptr };	// 頂点バッファ

	ID3D11Buffer* pIndexBuffer{ nullptr };	// インデックスバッファ


	// 頂点バッファ作成
	bool CreateVertexBuffer(D3D11_Renderer& _renderer);
	// インデックスバッファ（頂点1つ1つののID的なモノ)を作成
	bool CreateIndexBuffer(D3D11_Renderer& _renderer);
	// 削除関数
	void Release();

public:
	Mesh();
	~Mesh();

	// メッシュの読込
	bool Setup(D3D11_Renderer& _renderer, aiMesh* pMeshData);
	// 描画処理
	void Draw(D3D11_Renderer& _renderer);
};


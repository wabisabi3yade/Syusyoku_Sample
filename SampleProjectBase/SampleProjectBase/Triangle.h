#pragma once

// 3点の頂点からなる1つのポリゴンのクラス
class Triangle
{
	static constexpr int VERTEX_NUM = 3;	// 1ポリゴンの頂点数
	Vertex vertices[VERTEX_NUM];	// 頂点

	// Direct3Dのパイプラインに流す頂点バッファ
	ID3D11Buffer* pVertexBuffer = nullptr;	

	// ポリゴンの情報をシェーダーに渡す
	void SetupTransform(D3D11_Renderer& _renderer);

public:
	Triangle();
	~Triangle();

	// 頂点バッファを作成する
	bool CreateVertexBuffer(D3D11_Renderer& _renderer);

	// ポリゴンを描画する
	void Draw(D3D11_Renderer& _renderer);
};


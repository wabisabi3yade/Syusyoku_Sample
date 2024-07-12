#pragma once

// バッファ
#include "VertexBuffer.h"
#include "IndexBuffer.h"

// メッシュクラス
class Mesh
{
protected:
	/// @brief 頂点データ
	std::vector<Vertex> verticies;

	/// @brief 頂点バッファ
	std::unique_ptr<VertexBuffer> pVertexBuffer;

	/// @brief インデックス
	std::vector<u_int> indicies;

	/// @brief インデックスバッファ
	std::unique_ptr<IndexBuffer> pIndexBuffer;

	/// @brief 名前
	std::string name;

	/// @brief マテリアルインデックス
	u_int materialID;

	/// @brief トポロジー
	D3D11_PRIMITIVE_TOPOLOGY topology;

public:
	Mesh();
	Mesh(const Mesh& _other);
	virtual ~Mesh() {}

	Mesh& operator=(const Mesh& _other);

	/// @brief バッファの初期化
	void InitBuffer();

	/// @brief 頂点データを取得
	/// @return 頂点データ
	std::vector<Vertex>& GetVerticies(); 

	/// @brief 頂点バッファを取得
	/// @return 頂点バッファ
	const ID3D11Buffer& GetVertexBuffer() const;

	/// @brief インデックスバッファを取得
	/// @return インデックスバッファ
	const ID3D11Buffer& GetIndexBuffer() const;

	/// @brief インデックスを取得
	/// @return インデックスデータ
	std::vector<u_int>& GetIndicies();

	/// @brief インデックス数を取得
	/// @return インデックス数
	u_int GetIndexNum() const;

	/// @brief 名前を設定する
	/// @param _name 名前
	void SetName(const std::string& _name);

	/// @brief マテリアルのインデックスをセット
	/// @param _materialID マテリアルインデックス
	void SetMaterialID(u_int _materialID);

private:
	/// @brief コピー
	/// @param _other　対象 
	void Copy(const Mesh& _other);
};


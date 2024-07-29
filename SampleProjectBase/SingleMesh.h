#pragma once

#include "Vertex.h"

// バッファ
#include "VertexBuffer.h"
#include "IndexBuffer.h"

// 1つのメッシュクラス
class SingleMesh
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

	/// @brief 中心座標
	DirectX::SimpleMath::Vector3 centerPos;

	/// @brief メッシュのサイズ
	DirectX::SimpleMath::Vector3 size;

	// 初期設定
	void InitSetting(const std::string& _name, D3D11_PRIMITIVE_TOPOLOGY _topology);

public:
	SingleMesh();
	SingleMesh(const SingleMesh& _other);
	virtual ~SingleMesh() {}

	SingleMesh& operator=(const SingleMesh& _other);

	/// @brief バッファの初期化
	void InitBuffer();

	/// @brief 頂点データを取得
	/// @return 頂点データ
	std::vector<Vertex>& GetVerticies();

	/// @brief 頂点バッファを取得
	/// @return 頂点バッファ
	const VertexBuffer& GetVertexBuffer() const;

	/// @brief インデックスバッファを取得
	/// @return インデックスバッファ
	const IndexBuffer& GetIndexBuffer() const;

	/// @brief インデックスを取得
	/// @return インデックスデータ
	std::vector<u_int>& GetIndicies();

	/// @brief 頂点数を取得
	/// @return 頂点数
	u_int GetVertexNum() const;

	/// @brief インデックス数を取得
	/// @return インデックス数
	u_int GetIndexNum() const;

	/// @brief マテリアルインデックスを取得
	/// @return マテリアルインデックス
	u_int GetMaterialID() const;

	/// @brief 中心座標を取得する
	/// @return 中心座標
	DirectX::SimpleMath::Vector3 GetCenterPos();

	/// @brief サイズを取得する
	/// @return メッシュの大きさ
	DirectX::SimpleMath::Vector3 GetSize();

	/// @brief トポロジーを取得
	/// @return トポロジー
	D3D11_PRIMITIVE_TOPOLOGY GetTopology() const;

	/// @brief 名前を設定する
	/// @param _name 名前
	void SetName(const std::string& _name);

	/// @brief マテリアルのインデックスをセット
	/// @param _materialID マテリアルインデックス
	void SetMaterialID(u_int _materialID);

	/// @brief トポロジーをセット
	/// @param _topology トポロジー
	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology);

	/// @brief 中心座標をセット
	/// @param _centerPos 中心座標
	void SetCenterPosition(const DirectX::SimpleMath::Vector3& _centerPos);

	/// @brief サイズをセット
	/// @param _size モデルのサイズ
	void SetSize(const DirectX::SimpleMath::Vector3& _size);

protected:
	/// @brief コピー
	/// @param _other　対象
	void Copy(const SingleMesh& _other);
};

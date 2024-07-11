#pragma once

// メッシュクラス
class Mesh
{
protected:
	/// @brief 頂点データ
	std::vector<Vertex> verticies;

	/// @brief インデックス
	std::vector<u_int> indicies;

	/// @brief 名前
	std::string name;

	/// @brief マテリアルインデックス
	u_int materialID;

public:
	Mesh() : name(""), materialID(0) {}
	Mesh(const Mesh& _other);
	virtual ~Mesh() {}

	Mesh& operator=(const Mesh& _other);

	/// @brief 頂点データを取得
	/// @return 頂点データ
	std::vector<Vertex>& GetVerticies();

	/// @brief インデックスを取得
	/// @return インデックスデータ
	std::vector<u_int>& GetIndicies();

	/// @brief インデックス数を取得
	/// @return インデックス数
	u_int GetIndexNum();

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


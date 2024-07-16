#pragma once
#include "Asset_Base.h"

// 描画関連
#include "SingleMesh.h"
#include "Material.h"
#include "Texture.h"

// モデル（スケルタル・スタティックメッシュ）の基底クラス
class Mesh_Base : public Asset_Base
{
public:

	/// @brief メッシュの種類
	enum class Type
	{
		SM,	// スタティックメッシュ
		SK,	// スケルタルメッシュ
		None	// 設定なし
	};
private:
	/// @brief メッシュ配列
	std::vector<std::unique_ptr<SingleMesh>> pMeshes;

	/// @brief マテリアル配列
	std::vector<Material*> pMaterials;

	/// @brief 中心座標
	DirectX::SimpleMath::Vector3 centerPos;

	/// @brief モデルサイズ
	DirectX::SimpleMath::Vector3 size;

	/// @brief メッシュの種類
	Type meshType;
public:
	Mesh_Base() : meshType(Type::None) {}

	/// @brief コンストラクタ
	/// @param _meshType メッシュの種類
	Mesh_Base(Type _meshType) : meshType(_meshType) {}
	virtual ~Mesh_Base() {}
	
	/// @brief メッシュを追加
	/// @param _pMesh メッシュのユニークポインタ
	/// @param _materialID マテリアルインデックス
	void AddMesh(std::unique_ptr<SingleMesh> _pMesh);

	/// @brief マテリアルを追加
	/// @param _pMaterial セットするマテリアル
	void AddMaterial(Material* _pMaterial);

	/// @brief メッシュを取得
	/// @param _meshIdx 何個目
	/// @return メッシュ
	const SingleMesh* GetMesh(u_int _meshIdx) const;

	/// @brief メッシュの数取得
	/// @return メッシュの数
	u_int GetMeshNum();

	/// @brief マテリアル取得mesh
	/// @param _materialIdx マテリアルID
	/// @return マテリアルのポインタ
	Material* GetMaterial(u_int _materialIdx);

	/// @brief 中心座標を取得
	/// @return 中心座標
	DirectX::SimpleMath::Vector3 GetCenterPosition() const;

	/// @brief モデルのサイズを取得
	/// @return サイズ
	DirectX::SimpleMath::Vector3 GetSize() const;

	/// @brief モデルの中心座標をセット
	/// @param _size 中心座標
	void SetCenterPosition(const DirectX::SimpleMath::Vector3& _centerPos);

	/// @brief サイズをセット
	/// @param _size サイズ
	void SetSize(const DirectX::SimpleMath::Vector3& _size);
};


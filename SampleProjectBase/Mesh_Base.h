#pragma once
#include "Asset_Base.h"

// 描画関連
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

// メッシュ集合体（スケルタル・スタティックメッシュ）の基底クラス
class Mesh_Base : public Asset_Base
{
	/// @brief メッシュ配列
	std::vector<std::unique_ptr<Mesh>> pMeshes;

	/// @brief マテリアル配列
	std::vector<Material*> pMaterials;
public:

	/// @brief コンストラクタ
	/// @param _assetType アセットの種類
	Mesh_Base() {}
	virtual ~Mesh_Base() {}
	
	/// @brief メッシュを追加
	/// @param _pMesh メッシュのユニークポインタ
	/// @param _materialID マテリアルインデックス
	void AddMesh(std::unique_ptr<Mesh> _pMesh);

	/// @brief マテリアルを追加
	/// @param _pMaterial セットするマテリアル
	void AddMaterial(Material& _pMaterial);

	/// @brief メッシュの数取得
	/// @return メッシュの数
	u_int GetMeshNum();
};


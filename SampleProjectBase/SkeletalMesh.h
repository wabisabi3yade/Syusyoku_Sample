#pragma once
#include "Mesh_Group.h"

#include "Bone.h"
#include "TreeNode.h"

/// @brief ボーンリスト
class BoneList : public Asset_Base
{
	friend class AssetLoader;

	std::vector<std::unique_ptr<Bone>> pBones;
	
	/// @brief ボーン配列をセット
	/// @param _setList ボーン配列
	void SetBoneList(std::vector<std::unique_ptr<Bone>> _setList);

public:
	BoneList() {}
	~BoneList() {}

	/// @brief ボーンIDからボーンを取得する
	/// @param _boneIdx ボーンID
	/// @return ボーン
	Bone* FindBone(u_int _boneIdx);

	/// @brief 名前からボーンを取得する
	/// @param  _boneName ボーン名
	/// @return ボーン
	Bone* FindBone(const std::string& _boneName);

	// ボーンの数を返す 
	u_int GetBoneCnt() const;
};

// スケルタルメッシュクラス
class SkeletalMesh : public Mesh_Group
{
	/// @brief ボーン情報
	BoneList* pBoneList;

	/// @brief ノード情報の元ノードを持つ
	std::unique_ptr<TreeNode> pRootNode;

public:
	SkeletalMesh() : Mesh_Group(MeshType::SK), pBoneList(nullptr) {}
	~SkeletalMesh() {}

	/// @brief ボーン配列をセット
	/// @param _pBones ボーン配列
	void SetBoneList(BoneList* _pBones);

	// ルートノードをセット
	void SetRootNode(std::unique_ptr<TreeNode> _pRootNode);

	/// @brief 名前からボーンを探す
	/// @param _name 探すボーン名
	/// @return ボーンのポインタ
	Bone* GetBoneByName(const std::string& _name);

	/// @brief ボーンのIDから探す
	/// @param _idx ボーンID
	/// @return ボーンID
	Bone& GetBone(u_int _idx);

	// ボーンの数を取得
	u_int GetBoneNum();

	// ルートノードを取得
	TreeNode& GetRootNode();
};

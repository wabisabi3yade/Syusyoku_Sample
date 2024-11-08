#pragma once
#include "Mesh_Group.h"

#include "Bone.h"
#include "TreeNode.h"

/// @brief ボーンリスト
class BoneList : public Asset_Base
{
	friend class AssetLoader;

	/// @brief ボーン配列
	std::vector<std::unique_ptr<Bone>> pBones;

	/// @brief ロード時のオフセット回転量
	DirectX::SimpleMath::Quaternion loadOffsetRotation;

	/// @brief ロード時のスケール
	float loadScale;
	
	/// @brief ボーン配列をセット
	/// @param _setList ボーン配列
	void SetBoneList(std::vector<std::unique_ptr<Bone>> _setList);

public:
	BoneList();
	BoneList(const BoneList& _other);
	~BoneList() {}

	BoneList& operator=(const BoneList& _other);

	/// @brief ボーンIDからボーンを取得する
	/// @param _boneIdx ボーンID
	/// @return ボーン
	Bone* FindBone(u_int _boneIdx);

	/// @brief 名前からボーンを取得する
	/// @param  _boneName ボーン名
	/// @return ボーン
	Bone* FindBone(const std::string& _boneName);

	// 要素数からボーンを取得
	Bone& GetBone(u_int _arrayIdx);

	/// @brief 名前からボーンIDを取得する
	/// @param _boneName ボーン名
	/// @return ボーンID
	u_int GetIndex(const std::string& _boneName) const;

	// ボーンの数を返す 
	u_int GetBoneCnt() const;

	/// @brief ルートボーンのIDを取得する
	/// @return ルートボーンのID
	u_int GetRootBoneId() const;

	/// @brief ロード時のスケール倍率を取得
	/// @return ロード時のスケール倍率
	float GetLoadScale() const;

	/// @brief ロード時のスケール回転量を取得
	/// @return ロード時のスケール回転量
	const DirectX::SimpleMath::Quaternion& GetLoadRotation() const;

private:
	void Copy(const BoneList& _other);
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
	/// @return ボーン
	Bone* GetBone(u_int _idx);

	// ボーンの数を取得
	u_int GetBoneCnt();

	// ルートノードを取得
	TreeNode& GetRootNode();

	// ボーンリストを取得
	BoneList& GetBoneList();

};

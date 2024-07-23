#pragma once
#include "Mesh_Group.h"

// ボーン
#include "Bone.h"

// メッシュごとのボーン配列
using BonePerMesh = std::vector<std::unique_ptr<Bone>>;

// スケルタルメッシュクラス
class SkeletalMesh : public Mesh_Group
{
	/// @brief ボーン情報
	std::vector<BonePerMesh> bones;

public:
	SkeletalMesh() : Mesh_Group(MeshType::SK) {}
	~SkeletalMesh() {}

	/// @brief メッシュごとのボーン配列を追加
	/// @param _bones メッシュごとのボーン配列
	void AddBonePerMesh(BonePerMesh _bones);

	/// @brief ボーン配列のconst参照を取得する
	/// @return 配列のconst参照
	const std::vector<BonePerMesh>& GetBones();
};

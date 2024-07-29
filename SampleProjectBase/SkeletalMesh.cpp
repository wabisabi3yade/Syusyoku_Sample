#include "pch.h"
#include "SkeletalMesh.h"

void SkeletalMesh::AddBonePerMesh(BonePerMesh _bones)
{
	bones.push_back(std::move(_bones));
}

const std::vector<BonePerMesh>& SkeletalMesh::GetBones()
{
	return bones;
}

u_int SkeletalMesh::GetBoneNum()
{
	u_int boneCnt = 0;

	// メッシュごとのボーンの数を取得する
	for(u_int bm_i = 0; bm_i < static_cast<u_int>(bones.size()); bm_i++)
	{
		boneCnt += static_cast<u_int>(bones[bm_i].size());
	}

	return boneCnt;
}

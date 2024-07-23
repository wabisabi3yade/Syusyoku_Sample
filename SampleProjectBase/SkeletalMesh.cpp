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

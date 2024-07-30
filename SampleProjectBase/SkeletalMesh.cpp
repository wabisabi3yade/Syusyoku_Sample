#include "pch.h"
#include "SkeletalMesh.h"

void SkeletalMesh::SetBoneList(BoneList _pBones)
{
	pBones = std::move(_pBones);
}

void SkeletalMesh::SetRootNode(std::unique_ptr<TreeNode> _pRootNode)
{
	pRootNode = std::move(_pRootNode);
}

Bone* SkeletalMesh::GetBoneByName(const std::string& _name)
{
	for (auto& b : pBones)
	{
		if (b->GetBoneName() == _name)
			return b.get();
	}

	return nullptr;
}

Bone& SkeletalMesh::GetBone(u_int _idx)
{
	return *pBones[_idx];
}

u_int SkeletalMesh::GetBoneNum()
{
	return static_cast<u_int>(pBones.size());
}

TreeNode& SkeletalMesh::GetRootNode()
{
	return *pRootNode.get();
}

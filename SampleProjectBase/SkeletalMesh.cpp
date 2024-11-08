#include "pch.h"
#include "SkeletalMesh.h"

void SkeletalMesh::SetBoneList(BoneList* _pBones)
{
	pBoneList = _pBones;
}

void SkeletalMesh::SetRootNode(std::unique_ptr<TreeNode> _pRootNode)
{
	pRootNode = std::move(_pRootNode);
}

Bone* SkeletalMesh::GetBoneByName(const std::string& _name)
{
	return pBoneList->FindBone(_name);
}

Bone* SkeletalMesh::GetBone(u_int _idx)
{
	return pBoneList->FindBone(_idx);
}

u_int SkeletalMesh::GetBoneCnt()
{
	return pBoneList->GetBoneCnt();
}

TreeNode& SkeletalMesh::GetRootNode()
{
	return *pRootNode.get();
}

BoneList& SkeletalMesh::GetBoneList()
{
	return *pBoneList;
}

void BoneList::SetBoneList(std::vector<std::unique_ptr<Bone>> _setList)
{
	pBones = std::move(_setList);
}


BoneList::BoneList() : loadScale(1.0f)
{
}

BoneList::BoneList(const BoneList& _other)
{
	Copy(_other);
}

BoneList& BoneList::operator=(const BoneList& _other)
{
	Copy(_other);

	return *this;
}

Bone* BoneList::FindBone(u_int _boneIdx)
{
	assert(_boneIdx < GetBoneCnt() && "ボーンIDが大きすぎます");

	for (auto& b : pBones)
	{
		if (b->GetIndex() == _boneIdx)
			return b.get();
	}

	return nullptr;
}

Bone* BoneList::FindBone(const std::string& _boneName)
{
	for (auto& b : pBones)
	{
		if (b->GetBoneName() == _boneName)
			return b.get();
	}

	return nullptr;
}

Bone& BoneList::GetBone(u_int _arrayIdx)
{
	assert(_arrayIdx < GetBoneCnt());

	return *pBones[_arrayIdx];
}

u_int BoneList::GetIndex(const std::string& _boneName) const
{
	for (auto& b : pBones)
	{
		if (b->GetBoneName() == _boneName)
			return b->GetIndex();
	}

	assert("ボーンがありませんでした");
	HASHI_DEBUG_LOG(_boneName + "ボーンがありませんでした");

	return 0;
}

u_int BoneList::GetBoneCnt() const
{
	return static_cast<u_int>(pBones.size());
}

u_int BoneList::GetRootBoneId() const
{
	return 0;
}

float BoneList::GetLoadScale() const
{
	return loadScale;
}

const DirectX::SimpleMath::Quaternion& BoneList::GetLoadRotation() const
{
	return loadOffsetRotation;
}

void BoneList::Copy(const BoneList& _other)
{
	if (this == &_other) return;

	for (const auto& otherBone : _other.pBones)
	{
		std::unique_ptr<Bone> create = std::make_unique<Bone>(*otherBone.get());
		pBones.push_back(std::move(create));
	}
}

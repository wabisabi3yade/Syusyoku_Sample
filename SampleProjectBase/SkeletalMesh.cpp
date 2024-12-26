#include "pch.h"
#include "SkeletalMesh.h"

namespace HashiTaku
{
	void SkeletalMesh::SetAssetBoneList(BoneList* _pBones)
	{
		pAssetBoneList = _pBones;
	}

	void SkeletalMesh::SetRootNode(std::unique_ptr<TreeNode> _pRootNode)
	{
		pAssetBoneList->SetRootNode(std::move(_pRootNode));
	}

	Bone* SkeletalMesh::GetBoneByName(const std::string& _name)
	{
		return pAssetBoneList->FindBone(_name);
	}

	u_int SkeletalMesh::GetBoneCnt() const
	{
		return pAssetBoneList->GetBoneCnt();
	}

	const TreeNode* SkeletalMesh::GetRootNode() const
	{
		return pAssetBoneList->GetRootNode();
	}

	BoneList& SkeletalMesh::GetBoneList()
	{
		return *pAssetBoneList;
	}

	void BoneList::SetAssetBoneList(std::vector<std::unique_ptr<Bone>> _setList)
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

	Bone* BoneList::FindBone(const std::string& _boneName)
	{
		// ボーン名を探して返す
		for (auto& b : pBones)
		{
			if (b->GetBoneName() == _boneName)
				return b.get();
		}

		return nullptr;
	}

	void BoneList::SetRootNode(std::unique_ptr<TreeNode> _pRootNode)
	{
		pRootNode = std::move(_pRootNode);
	}

	Bone* BoneList::GetBone(u_int _boneId)
	{
#ifdef EDIT
		// ボーンIDが大きすぎたら
		if (_boneId >= GetBoneCnt())
		{
			assert(!"ボーンIDが大きすぎます");
			return nullptr;
		}

		// 配列の要素数とボーンインデックスが一致していないなtら
		if (pBones[_boneId]->GetIndex() != _boneId)
		{
			assert(!"ボーンIDが対応できていません");
			return nullptr;
		}
#endif // EDIT	

		return pBones[_boneId].get();
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

	const TreeNode* BoneList::GetRootNode() const
	{
		return pRootNode.get();
	}

	float BoneList::GetLoadScale() const
	{
		return loadScale;
	}

	const DXSimp::Quaternion& BoneList::GetLoadRotation() const
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

		loadOffsetRotation = _other.loadOffsetRotation;
		loadScale = _other.loadScale;
	}
}
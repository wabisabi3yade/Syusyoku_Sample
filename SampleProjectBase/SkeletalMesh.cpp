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
		// �{�[������T���ĕԂ�
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
		// �{�[��ID���傫��������
		if (_boneId >= GetBoneCnt())
		{
			assert(!"�{�[��ID���傫�����܂�");
			return nullptr;
		}

		// �z��̗v�f���ƃ{�[���C���f�b�N�X����v���Ă��Ȃ���t��
		if (pBones[_boneId]->GetIndex() != _boneId)
		{
			assert(!"�{�[��ID���Ή��ł��Ă��܂���");
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

		assert("�{�[��������܂���ł���");
		HASHI_DEBUG_LOG(_boneName + "�{�[��������܂���ł���");

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
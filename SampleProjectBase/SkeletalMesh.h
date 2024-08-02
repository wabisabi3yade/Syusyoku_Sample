#pragma once
#include "Mesh_Group.h"

#include "Bone.h"
#include "TreeNode.h"

/// @brief �{�[�����X�g
class BoneList : public Asset_Base
{
	friend class AssetLoader;

	std::vector<std::unique_ptr<Bone>> pBones;
	
	/// @brief �{�[���z����Z�b�g
	/// @param _setList �{�[���z��
	void SetBoneList(std::vector<std::unique_ptr<Bone>> _setList);

public:
	BoneList() {}
	~BoneList() {}

	/// @brief �{�[��ID����{�[�����擾����
	/// @param _boneIdx �{�[��ID
	/// @return �{�[��
	Bone* FindBone(u_int _boneIdx);

	/// @brief ���O����{�[�����擾����
	/// @param  _boneName �{�[����
	/// @return �{�[��
	Bone* FindBone(const std::string& _boneName);

	// �{�[���̐���Ԃ� 
	u_int GetBoneCnt() const;
};

// �X�P���^�����b�V���N���X
class SkeletalMesh : public Mesh_Group
{
	/// @brief �{�[�����
	BoneList* pBoneList;

	/// @brief �m�[�h���̌��m�[�h������
	std::unique_ptr<TreeNode> pRootNode;

public:
	SkeletalMesh() : Mesh_Group(MeshType::SK), pBoneList(nullptr) {}
	~SkeletalMesh() {}

	/// @brief �{�[���z����Z�b�g
	/// @param _pBones �{�[���z��
	void SetBoneList(BoneList* _pBones);

	// ���[�g�m�[�h���Z�b�g
	void SetRootNode(std::unique_ptr<TreeNode> _pRootNode);

	/// @brief ���O����{�[����T��
	/// @param _name �T���{�[����
	/// @return �{�[���̃|�C���^
	Bone* GetBoneByName(const std::string& _name);

	/// @brief �{�[����ID����T��
	/// @param _idx �{�[��ID
	/// @return �{�[��ID
	Bone& GetBone(u_int _idx);

	// �{�[���̐����擾
	u_int GetBoneNum();

	// ���[�g�m�[�h���擾
	TreeNode& GetRootNode();
};

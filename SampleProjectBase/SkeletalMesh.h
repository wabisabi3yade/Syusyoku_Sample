#pragma once
#include "Mesh_Group.h"

#include "Bone.h"
#include "TreeNode.h"

using BoneList = std::vector<std::unique_ptr<Bone>>;

// �X�P���^�����b�V���N���X
class SkeletalMesh : public Mesh_Group
{
	/// @brief �{�[�����
	BoneList pBones;

	/// @brief �m�[�h���̌��m�[�h������
	std::unique_ptr<TreeNode> pRootNode;

public:
	SkeletalMesh() : Mesh_Group(MeshType::SK) {}
	~SkeletalMesh() {}

	/// @brief �{�[���z����Z�b�g
	/// @param _pBones �{�[���z��
	void SetBoneList(BoneList _pBones);

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

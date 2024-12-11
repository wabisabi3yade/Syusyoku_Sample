#pragma once
#include "Mesh_Group.h"

#include "Bone.h"
#include "TreeNode.h"

/// @brief �{�[�����X�g
class BoneList : public Asset_Base
{
	friend class AssetLoader;

	/// @brief �{�[���z��
	std::vector<std::unique_ptr<Bone>> pBones;

	/// @brief ���[�h���̃I�t�Z�b�g��]��
	DirectX::SimpleMath::Quaternion loadOffsetRotation;

	/// @brief �m�[�h���̌��m�[�h������
	std::unique_ptr<TreeNode> pRootNode;

	/// @brief ���[�h���̃X�P�[��
	float loadScale;
	
	/// @brief �{�[���z����Z�b�g
	/// @param _setList �{�[���z��
	void SetAssetBoneList(std::vector<std::unique_ptr<Bone>> _setList);

public:
	BoneList();
	BoneList(const BoneList& _other);
	~BoneList() {}

	BoneList& operator=(const BoneList& _other);

	/// @brief ���O����{�[�����擾����
	/// @param  _boneName �{�[����
	/// @return �{�[��
	Bone* FindBone(const std::string& _boneName);

	/// @brief ���[�g�m�[�h���Z�b�g
	/// @param �X�P���^�����b�V���̃��[�g�m�[�h���擾
	void SetRootNode(std::unique_ptr<TreeNode> _pRootNode);

	// �v�f������{�[�����擾
	Bone* GetBone(u_int _arrayIdx);

	/// @brief ���O����{�[��ID���擾����
	/// @param _boneName �{�[����
	/// @return �{�[��ID
	u_int GetIndex(const std::string& _boneName) const;

	// �{�[���̐���Ԃ� 
	u_int GetBoneCnt() const;

	/// @brief ���[�g�{�[����ID���擾����
	/// @return ���[�g�{�[����ID
	u_int GetRootBoneId() const;

	/// @brief ���[�g�m�[�h���擾����
	/// @return ���[�g�m�[�h
	const TreeNode* GetRootNode() const;

	/// @brief ���[�h���̃X�P�[���{�����擾
	/// @return ���[�h���̃X�P�[���{��
	float GetLoadScale() const;

	/// @brief ���[�h���̃X�P�[����]�ʂ��擾
	/// @return ���[�h���̃X�P�[����]��
	const DirectX::SimpleMath::Quaternion& GetLoadRotation() const;

private:
	void Copy(const BoneList& _other);
};

// �X�P���^�����b�V���N���X
class SkeletalMesh : public Mesh_Group
{
	friend class AssetLoader;

	/// @brief ���̃X�P���^�����b�V���Ŏg�p����{�[�����X�g�̃|�C���^(�A�Z�b�g�Ǘ��ɂ���)
	BoneList* pAssetBoneList;

public:
	SkeletalMesh() : Mesh_Group(MeshType::SK), pAssetBoneList(nullptr) {}
	~SkeletalMesh() {}

	/// @brief ���O����{�[����T��
	/// @param _name �T���{�[����
	/// @return �{�[���̃|�C���^
	Bone* GetBoneByName(const std::string& _name);

	// �{�[���̐����擾
	u_int GetBoneCnt() const;

	/// @brief ���[�g�m�[�h���擾����
	/// @return ���[�g�m�[�h
	const TreeNode* GetRootNode() const;

	// �{�[�����X�g���擾(����͓������Ȃ��悤�ɂ���)
	BoneList& GetBoneList();

private:
	/// @brief ���[�g�m�[�h���Z�b�g
	/// @param �X�P���^�����b�V���̃��[�g�m�[�h���擾
	void SetRootNode(std::unique_ptr<TreeNode> _pRootNode);

	/// @brief �{�[���z����Z�b�g
	/// @param _pBones �{�[���z��
	void SetAssetBoneList(BoneList* _pBones);
};

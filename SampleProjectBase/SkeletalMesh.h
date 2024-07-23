#pragma once
#include "Mesh_Group.h"

// �{�[��
#include "Bone.h"

// ���b�V�����Ƃ̃{�[���z��
using BonePerMesh = std::vector<std::unique_ptr<Bone>>;

// �X�P���^�����b�V���N���X
class SkeletalMesh : public Mesh_Group
{
	/// @brief �{�[�����
	std::vector<BonePerMesh> bones;

public:
	SkeletalMesh() : Mesh_Group(MeshType::SK) {}
	~SkeletalMesh() {}

	/// @brief ���b�V�����Ƃ̃{�[���z���ǉ�
	/// @param _bones ���b�V�����Ƃ̃{�[���z��
	void AddBonePerMesh(BonePerMesh _bones);

	/// @brief �{�[���z���const�Q�Ƃ��擾����
	/// @return �z���const�Q��
	const std::vector<BonePerMesh>& GetBones();
};

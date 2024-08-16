#pragma once
#include "Component.h"
#include "IAnimationObserver.h"

// ���f��
class SkeletalMesh;
class Bone;
class TreeNode;

class AnimationController;

constexpr u_int MAX_BONEMTX(400);	// �V�F�[�_�[�̓n���{�[���s��̍ő吔

/// @brief �A�j���[�V�����R���|�[�l���g
class CP_Animation : public Component
{
	// �V�F�[�_�[�ɓn���{�[���s��\����
	struct BoneCombMtricies
	{
		DirectX::SimpleMath::Matrix matrix[MAX_BONEMTX];
	};

	BoneCombMtricies boneComb;

	/// @brief �X�P���^�����b�V��
	SkeletalMesh* pSkeletalMesh;

	/// @brief �A�j���[�V�����R���g���[���[
	AnimationController* pAnimController;
public:
	CP_Animation();
	~CP_Animation() {}

	void Init() override;

	void LateUpdate() override;

	void ImGuiSetting() override;

	/// @brief �X�P���^�����b�V�����Z�b�g
	/// @param _skeletalMesh 
	void SetSkeletalMesh(SkeletalMesh& _skeletalMesh);

	// �A�j���[�V�����R���g���[���[���Z�b�g
	void SetAnimationController(AnimationController& _controller);

	// �X�P���^�����b�V�����擾
	SkeletalMesh& GetSkeletalMesh();
private:
	/// @brief �Đ��ł����Ԃ��H
	/// @return �Đ��ł��邩
	bool IsCanPlay();

	/// @brief �{�[���R���r�l�[�V�����s����X�V
	void UpdateBoneCombMtx();

	/// @brief �q�m�[�h�̃R���r�l�[�V�����s����X�V����i�ċA�֐��j
	/// @param _treeNode �X�V����m�[�h
	/// @param _parentMtx ���[���h�ϊ����邽�߂̐e�܂ł̍s��
	void UpdateNodeHierarchy(TreeNode& _treeNode, const DirectX::SimpleMath::Matrix& _parentMtx);

	/// @brief �{�[���̃R���r�l�[�V�����s����X�V
	void UpdateAnimationMtx();

	/// @brief �V�F�[�_�[�̃o�b�t�@���X�V����
	void UpdateBoneBuffer();
};


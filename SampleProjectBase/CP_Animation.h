#pragma once
#include "Component.h"

// ���f��
class SkeletalMesh;
class Bone;
class TreeNode;

// �A�j���[�V�����f�[�^
class AnimationData;

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

	/// @brief ���݂̃A�j���[�V����
	AnimationData* pCurrentAnimation;

	/// @brief ���̃R���|�[�l���g�ɂ���A�j���[�V����
	std::list<AnimationData*> pHaveAnimations;

	/// @brief ���ݍĐ����Ă���A�j���[�V�����̎���(�P�ʁFs)
	float playingTime_s;

	/// @brief �Đ����x
	float playSpeed;

	/// @brief �Đ������H
	bool isPlaying;
public:
	CP_Animation() : pSkeletalMesh(nullptr), pCurrentAnimation(nullptr), playingTime_s(0.0f),
		playSpeed(1.0f) , isPlaying(false) {}
	~CP_Animation() {}

	void Init() override;

	void LateUpdate() override;

	void ImGuiSetting() override;

	/// @brief �A�j���[�V�������Đ�����
	/// @param _animName �Đ��������A�j���[�V�����̖��O
	void PlayAnimation(const std::string& _animName);

	/// @brief �A�j���[�V������ǉ�����
	/// @param _addAnim �ǉ�����A�j���[�V����
	void AddAnimations(AnimationData& _addAnim);

	/// @brief �A�j���[�V������ǉ�����
	/// @param _animName �ǉ�����A�j���[�V�����̖��O
	void AddAnimations(const std::string& _animName);

	/// @brief �A�j���[�V�������O��
	/// @param _animName �O�������A�j���[�V�����̖��O
	void RemoveAnimations(const std::string& _animName);

	/// @brief �X�P���^�����b�V�����Z�b�g
	/// @param _skeletalMesh 
	void SetSkeletalMesh(SkeletalMesh& _skeletalMesh);
private:

	/// @brief �Đ����Ԃ�i�߂�
	void ProgressPlayTime();

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

	/// @brief �������Ă���A�j���[�V������T��
	/// @param _animName 
	/// @return 
	AnimationData* FindAnimaton(const std::string& _animName);
};


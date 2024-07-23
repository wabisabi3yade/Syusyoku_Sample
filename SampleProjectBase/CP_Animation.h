#pragma once
#include "Component.h"

// ���f��
class SkeletalMesh;
class Bone;

// �A�j���[�V�����f�[�^
class AnimationData;

/// @brief �A�j���[�V�����R���|�[�l���g
class CP_Animation : public Component
{
	/// @brief �X�P���^�����b�V��
	SkeletalMesh* pSkeletalMesh;

	/// @brief ���݂̃A�j���[�V����
	AnimationData* pCurrentAnimation;

	/// @brief ���̃R���|�[�l���g�ɂ���A�j���[�V����
	std::list<AnimationData*> pHaveAnimations;

	/// @brief �Đ������H
	bool isPlaying;

public:
	CP_Animation() : pSkeletalMesh(nullptr), pCurrentAnimation(nullptr), isPlaying(false) {}
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

	/// @brief �A�j���[�V�������X�V����
	void UpdateAnimation();

	// ����
	const Bone* GetBoneByName(const std::string& _boneName);

	/// @brief �������Ă���A�j���[�V������T��
	/// @param _animName 
	/// @return 
	AnimationData* FindAnimaton(const std::string& _animName);

	/// @brief �A�j���[�V�����̏��Ƀ{�[����ID�������N������
	/// @param _connectAnim �����N����A�j���[�V����
	void ConnectBoneId(AnimationData& _connectAnim);
};


#pragma once
#include "Asset_Base.h"

#include "AnimStateNode.h"
#include "InertInterpAnimation.h"
// vector�z��Ŏg�p���邽��include
#include "Bone.h"

// �{�[��
class BoneList;

/// @brief �A�j���[�V�����J�ڂ��Ǘ�����N���X
class AnimationController : public Asset_Base
{
	/// @brief �A�j���[�V�����m�[�h�z��
	std::unordered_map<std::string, std::unique_ptr<AnimStateNode>> pAnimationNodes;

	/// @brief �Đ����̃A�j���[�V����
	AnimStateNode* pCurrentAnimNode;

	/// @brief �J�ڐ�̃A�j���[�V����
	AnimStateNode* pNextAnimNode;

	/// @brief �������
	std::unique_ptr<InertInterpAnimation> inertInterp;

	// ��
	float blendTime = 0.5f;

	// �u�����h���Ă��鎞��
	float blendElapsedTime = 0.0f;

	/// @brief �A�j���[�V�����J�ڒ����H
	bool isTransitioning;
public:
	AnimationController();
	~AnimationController() {}

	/// @brief �{�[���̃A�j���[�V�������X�V����
	/// @param _boneList �{�[�����X�g
	/// @param _playingTime �Đ�����
	void Update(BoneList& _boneList, float _playingTime);

	/// @brief �ʏ펞�A�A�j���[�V����
	/// @param _boneList �{�[�����X�g
	/// @param _playingTime �Đ�����
	/// @param _cacheTransform �L���b�V���p�̔z��Q��
	void NormalUpdate(BoneList& _boneList, float& _playingTime, std::vector<BoneTransform>& _cacheTransform);

	/// @brief �J�ڎ��̃A�j���[�V����
	/// @param _boneList �{�[�����X�g
	/// @param _playingTime �Đ�����
	/// @param _cacheTransform �L���b�V���p�̔z��Q��
	void TransitionUpdate(BoneList& _boneList, float& _playingTime, std::vector<BoneTransform>& _cacheTransform);

	void ImGuiSetting();

	void ChangeAnimation(const std::string& _animName);
	
	/// @brief �A�j���[�V������ǉ�
	/// @param _animName �A�j���[�V�����̖��O
	void AddAnimation(const std::string& _animName);

	/// @brief �A�j���[�V���������O
	/// @param _animName �A�j���[�V�����̖��O
	void RemoveAnimation(const std::string& _animName);

	/// @brief �A�j���[�V�������Z�b�g����Ă��邩�Ԃ�
	/// @return �A�j���[�V�������Z�b�g����Ă��邩�H
	bool IsSetAnimation();

	/// @brief �Đ����̃m�[�h���擾����
	/// @return �A�j���[�V�����m�[�h
	AnimStateNode& GetCurrentNode();
private:
	void ImGuiTransition();

	/// @brief �A�j���[�V�����������Ă邩�Ԃ�
	/// @param _animName �A�j���[�V�����̖��O
	/// @return �A�j���[�V���������邩�H
	bool IsHaveAnim(const std::string& _animName);

	/// @brief �J�ڏI���������̏���
	void TransitionEnd(float& _playTime);
};


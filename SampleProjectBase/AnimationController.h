#pragma once
#include "Asset_Base.h"

#include"AnimationNode_Base.h"
// �������
#include "InertInterpAnimation.h"
// vector�z��Ŏg�p���邽��include
#include "Bone.h"

// �{�[��
class BoneList;

/// @brief �A�j���[�V�����J�ڂ��Ǘ�����N���X
class AnimationController : public Asset_Base
{
	/// @brief �J�ڐ�̃A�j���[�V����
	AnimationNode_Base* pNextAnimNode;

	/// @brief �������
	std::unique_ptr<InertInterpAnimation> inertInterp;

	/// @brief �Đ�����
	float playingRatio;

	/// @brief �Đ����x
	float playSpeed;

	// ��
	float blendTime = 0.5f;

	// �u�����h���Ă��鎞��
	float blendElapsedTime;

	/// @brief �������{�[���z��
	BoneList* pBoneList;

	/// @brief �Đ����邩
	bool isPlay;

	/// @brief �A�j���[�V�����J�ڒ����H
	bool isTransitioning;

protected:
	/// @brief �A�j���[�V�����m�[�h�z��
	std::unordered_map<std::string, std::unique_ptr<AnimationNode_Base>> pAnimationNodes;

	/// @brief �Đ����̃A�j���[�V����
	AnimationNode_Base* pCurrentAnimNode;

public:
	/// @brief �R���X�g���N�^
	/// @param _boneCnt �{�[����
	AnimationController();
	~AnimationController() {}

	/// @brief �{�[���̃A�j���[�V�������X�V����
	/// @param _boneList �{�[�����X�g
	/// @param _playingTime �Đ�����
	void Update(BoneList& _boneList);

	void ImGuiSetting();

	/// @brief �A�j���[�V�����J�ڂ���
	/// @param _animName �A�j���[�V������
	/// @param  _isInterp ��Ԃ��邩�H
	void ChangeAnimation(const std::string& _animName, bool _isInterp = true);

	/// @brief �u�����h�������Z�b�g
	/// @param _ratio ����
	void SetBlendRatio(float _ratio);

	// �������{�[���z����Z�b�g
	void SetBoneList(BoneList& _boneList);

	/// @brief �V���O���m�[�h���쐬
	/// @param _nodeName �m�[�h��
	/// @param _animName �Z�b�g����A�j���[�V������
	void CreateSingleNode(const std::string& _nodeName, const std::string& _animName);

	/// @brief �u�����h�m�[�h���쐬����
	/// @param _animNames �A�j���[�V�����̖��O
	/// @param _ratios�u�����h����
	/// @param _nodeName �m�[�h��
	void CreateBlendNode(const std::vector<std::string>& _animNames, const std::vector<float>& _ratios, const std::string& _nodeName);

	/// @brief �A�j���[�V���������O
	/// @param _animName �A�j���[�V�����̖��O
	void RemoveAnimation(const std::string& _animName);

	/// @brief �A�j���[�V�������Z�b�g����Ă��邩�Ԃ�
	/// @return �A�j���[�V�������Z�b�g����Ă��邩�H
	bool IsSetAnimation();

	AnimationNode_Base* GetCurrentNode();

	AnimationNode_Base* GetNode(const std::string& _name);
private:
	/// @brief �Đ����Ԃ�i�߂�
	void ProgressPlayTime();

	/// @brief �Đ��ł����Ԃ��H
	/// @return �Đ��ł��邩
	bool IsCanPlay();

	/// @brief �A�j���[�V�����̍X�V����
	/// @param _boneList �X�V�{�[�����X�g
	void AnimatioUpdate();

	/// @brief ���[�v�Đ��ł��邩�H
	/// @return �Đ��ł���t���O
	bool IsCanLoop();

	/// @brief �ʏ펞�A�A�j���[�V����
	/// @param _boneList �{�[�����X�g
	void NormalUpdate();

	/// @brief �J�ڎ��̃A�j���[�V����
	/// @param _boneList �{�[�����X�g
	void TransitionUpdate();

	/// @brief ������Ԃ̃L���b�V���X�V
	void CacheUpdate();

	void ImGuiTransition();

	void ImGuiImportAnim();

	/// @brief �A�j���[�V�����������Ă邩�Ԃ�
	/// @param _animName �A�j���[�V�����̖��O
	/// @return �A�j���[�V���������邩�H
	bool IsHaveAnim(const std::string& _animName);

	/// @brief �J�ڊJ�n����
	/// @param _animName �A�j���[�V������
	void InterpTransitionStart(const std::string& _animName);

	/// @brief �J�ڏI���������̏���
	void InterpTransitionEnd();

protected:
	/// @brief �A�j���[�V�����I��������
	virtual void OnAnimationFinish();
};


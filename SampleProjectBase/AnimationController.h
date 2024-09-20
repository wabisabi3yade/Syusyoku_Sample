#pragma once
#include "Asset_Base.h"
#include"AnimationNode_Base.h"
#include "AnimConntrollerType.h"

// �N���X�t�F�[�h�E�������
#include "CrossFadeAnimation.h"
#include "InertInterpAnimation.h"
// vector�z��Ŏg�p���邽��include
#include "Bone.h"

// �{�[��
class BoneList;

/// @brief �A�j���[�V�����J�ڂ��Ǘ�����N���X
class AnimationController : public Asset_Base, public HashiTaku::IImGuiUser
{
	/// @brief �A�j���[�V�����m�[�h�֘A�̏��
	struct AnimNodeInfo
	{
		std::unique_ptr<AnimationNode_Base> pAnimNode;	// �A�j���[�V�����m�[�h
		std::list<std::unique_ptr<AnimTransitionArrow>> pTransArrows; // �J�ڌ��ƂȂ��Ă�����
	};

	/// @brief �N���X�t�F�[�h
	std::unique_ptr<CrossFadeAnimation> pCrossFadeInterp;

	/// @brief �������
	std::unique_ptr<InertInterpAnimation> pInertInterp;

	/// @brief �������{�[���z��
	BoneList* pBoneList;

	/// @brief ���J�ڂŎg�p���Ă�����(nullptr�Ȃ�J�ڂ��Ă��Ȃ�)
	const AnimTransitionArrow* pCurTransArrow;

	/// @brief �A�j���[�V�����R���g���[���[�^�C�v
	AnimConType controllerType;

	/// @brief �Đ�����
	float playingRatio;

	/// @brief �Đ����x
	float playSpeed;

	/// @brief �Đ����邩
	bool isPlay;

	/// @brief �A�j���[�V�����J�ڒ����H
	bool isTransitioning;

protected:
	/// @brief �A�j���[�V�����m�[�h�z��
	std::unordered_map<std::string, std::unique_ptr<AnimationNode_Base>> pAnimationNodes;

	/// @brief �A�j���[�V�����m�[�h�֘A�̏�񃊃X�g
	std::list<AnimNodeInfo> animaNodeInfos;

	/// @brief �O�̃A�j���[�V����
	AnimationNode_Base* pPrevAnimNode;

	/// @brief �Đ����̃A�j���[�V����
	AnimationNode_Base* pCurrentAnimNode;

public:
	/// @brief �R���X�g���N�^
	/// @param _setType �R���g���[���[�̎��
	AnimationController(AnimConType _setType = AnimConType::Default);
	~AnimationController() {}

	/// @brief �{�[���̃A�j���[�V�������X�V����
	/// @param _boneList �{�[�����X�g
	/// @param _playingTime �Đ�����
	void Update(BoneList& _boneList);

	/// @brief �A�j���[�V�����J�ڂ���
	/// @param _animName �A�j���[�V������
	/// @param _transitionArrow ����g�p����J�ږ��
	virtual void ChangeAnimation(const std::string& _animName, const AnimTransitionArrow* _transitionArrow);

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

	/// @brief �A�j���[�V�����J�ڂ̖����쐬����
	/// @param _fromNodeName �J�ڌ��A�j���[�V����
	/// @param _toNodeName �J�ڐ�A�j���[�V����
	/// @param _targetAnimRatio �^�[�Q�b�g�̃A�j���[�V��������
	/// @param  _transitionTime �J�ڎ���
	/// @param _condition �J�ڏ���
	/// @return �쐬�������
	AnimTransitionArrow* CreateTransitionArrow(const std::string& _fromNodeName, const std::string& _toNodeName, float _targetAnimRatio, float _transitionTime, std::function<bool()> _condition);

	/// @brief �A�j���[�V���������O
	/// @param _animName �A�j���[�V�����̖��O
	void RemoveAnimation(const std::string& _animName);

	/// @brief �A�j���[�V�������Z�b�g����Ă��邩�Ԃ�
	/// @return �A�j���[�V�������Z�b�g����Ă��邩�H
	bool IsSetAnimation();

	AnimationNode_Base* GetCurrentNode();

	AnimationNode_Base* GetNode(const std::string& _name);

	AnimConType GetControllerType() const;

	// ���݂̍Đ��������擾
	float GetPlayingRatio() const;

	// �Đ����Ă��邩�擾
	bool GetIsPlay() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �Đ����Ԃ�i�߂�
	void ProgressPlayTime();

	/// @brief �Đ��ł����Ԃ��H
	/// @return �Đ��ł��邩
	bool IsCanPlay();

	/// @brief �A�j���[�V�����̍X�V����
	void AnimatioUpdate();

	/// @brief ���[�v�Đ��ł��邩�H
	/// @return �Đ��ł���t���O
	bool IsCanLoop();

	/// @brief �ʏ펞�A�A�j���[�V����
	void NormalUpdate();

	/// @brief �N���X�t�F�[�h��Ԏ��̃A�j���[�V����
	void CrossFadeUpdate();

	/// @brief ������ԑJ�ڎ��̃A�j���[�V����
	void InertInterpUpdate();

	/// @brief ������Ԃ̃L���b�V���X�V
	void CacheUpdate();

	/// @brief �J�ڂ��邩�m�F����
	void TranstionCheck();

	/// @brief �A�j���[�V�����������Ă邩�Ԃ�
	/// @param _animName �A�j���[�V�����̖��O
	/// @return �A�j���[�V���������邩�H
	bool IsHaveAnim(const std::string& _animName);

	/// @brief ���ʂ����J�ڊJ�n����
	void OnTransitionStart();

	/// @brief �N���X�t�F�[�h��Ԃ��J�n
	/// @param _changePlayRatio �ύX���̑J�ڌ��̃A�j���[�V��������
	/// @param _targetAnimRatio �J�ڏI�����̑J�ڐ�̃A�j���[�V��������
	/// @param _transitionTime �J�ڎ���
	/// @param _easeKind �J�ڂ̃C�[�W���O
	void CrossFadeStart(float _changePlayRatio, float _targetAnimRatio, float _transitionTime, HashiTaku::EaseKind _easeKind);

	/// @brief ������ԊJ�n����
	/// @param_targetAnimRatio �J�ڐ�̃A�j���[�V��������
	/// @param _transitionTime �J�ڎ���
	void InterpTransitionStart(float _targetAnimRatio, float _transitionTime);

	/// @brief �J�ڏI���������̏���
	void OnTransitionEnd();

	void ImGuiTransition();
	void ImGuiImportAnim();

protected:
	void ImGuiSetting() override;

	/// @brief �A�j���[�V�����I��������
	virtual void OnAnimationFinish();

	/// @brief �A�j���[�V������ύX����
	void OnChangeAnimComplete();
};


#pragma once
#include "Asset_Base.h"
#include"AnimationNode_Base.h"
#include "AnimConntrollerType.h"
#include "AnimationParameters.h"

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

	/// @brief �A�j���[�V�����m�[�h�֘A�̏�񃊃X�g
	std::list<std::unique_ptr<AnimNodeInfo>> animNodeInfos;

	/// @brief �p�����[�^���X�g
	std::unique_ptr<AnimationParameters> pAnimParameters;

	/// @brief �O�̃A�j���[�V����
	AnimationNode_Base* pPrevAnimNode;

	/// @brief �ŏ��Ɏn�߂�m�[�h���
	AnimNodeInfo* pDefaultNodeInfo;

	/// @brief �������{�[���z��
	BoneList* pBoneList;

	/// @brief ���J�ڂŎg�p���Ă�����(nullptr�Ȃ�J�ڂ��Ă��Ȃ�)
	const AnimTransitionArrow* pCurTransArrow;

	/// @brief �N���X�t�F�[�h
	std::unique_ptr<CrossFadeAnimation> pCrossFadeInterp;

	/// @brief �������
	std::unique_ptr<InertInterpAnimation> pInertInterp;

	/// @brief �Đ����x
	float playSpeed;

	/// @brief �Đ����邩
	bool isPlay;

	/// @brief �A�j���[�V�����J�ڒ����H
	bool isTransitioning;

protected:
	/// @brief ���݂̃m�[�h���
	AnimNodeInfo* pCurrentNodeInfo;

public:
	/// @brief �R���X�g���N�^
	/// @param _setType �R���g���[���[�̎��
	AnimationController();
	~AnimationController() {}
	AnimationController(const AnimationController& _other);
	
	AnimationController& operator=(const AnimationController& _other);

	/// @brief �J�n����
	void Begin(BoneList& _boneList);

	/// @brief �{�[���̃A�j���[�V�������X�V����
	/// @param _boneList �{�[�����X�g
	/// @param _playingTime �Đ�����
	void Update(BoneList& _boneList);

	/// @brief �A�j���[�V�����J�ڂ���
	/// @param _animName �A�j���[�V������
	/// @param _transitionArrow ����g�p����J�ږ��
	virtual void ChangeAnimation(const std::string& _animName);

	virtual void ChangeAnimation(const AnimTransitionArrow& _transitionArrow);

	/// @brief �u�����h�������Z�b�g
	/// @param _ratio ����
	void SetBlendRatio(float _ratio);

	// �������{�[���z����Z�b�g
	void SetBoneList(BoneList& _boneList);

	/// @brief �J�n���̃f�t�H���g�m�[�h���Z�b�g����
	/// @param _nodeName �m�[�h��
	void SetDefaultNode(const std::string& _nodeName);

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
	void RemoveNode(const std::string& _animName);

	/// @brief �A�j���[�V�������Z�b�g����Ă��邩�Ԃ�
	/// @return �A�j���[�V�������Z�b�g����Ă��邩�H
	bool IsSetAnimation();

	// ���݂̃m�[�h���擾����
	AnimationNode_Base* GetCurrentNode();
	
	// ���O����m�[�h�����擾 
	AnimNodeInfo* GetNodeInfo(const std::string& _name);

	// �m�[�h����m�[�h�����擾����
	AnimNodeInfo* GetNodeInfo(const AnimationNode_Base& _node);

	// ���݂̍Đ��������擾
	float GetPlayingRatio() const;

	/// @brief �m�[�h�̐����擾
	/// @return �m�[�h��
	void GetNodeArray(std::list<const AnimationNode_Base*>& _animNodeArray) const;

	// �Đ����Ă��邩�擾
	bool GetIsPlay() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
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

	/// @brief ���ʂ����J�ڊJ�n����
	void OnTransitionStart();

	/// @brief ���O�̃m�[�h�����ɂ��邩�m�F����
	/// @param _nodeName �m�F���閼�O
	/// @return �m�[�h�������Ă��邩�H
	bool IsHaveNode(const std::string& _nodeName);

	/// @brief �m�[�h�����d�����Ȃ��悤�Ȗ��O�ɕϊ�
	/// @param _nodename �m�[�h��
	void NotDuplicateNodeName(std::string& _nodename);

	/// @brief �N���X�t�F�[�h��Ԃ��J�n
	/// @param _targetAnimRatio �J�ڏI�����̑J�ڐ�̃A�j���[�V��������
	/// @param _transitionTime �J�ڎ���
	/// @param _easeKind �J�ڂ̃C�[�W���O
	void CrossFadeStart(float _targetAnimRatio, float _transitionTime, HashiTaku::EaseKind _easeKind);

	/// @brief ������ԊJ�n����
	/// @param_targetAnimRatio �J�ڐ�̃A�j���[�V��������
	/// @param _transitionTime �J�ڎ���
	void InterpTransitionStart(float _targetAnimRatio, float _transitionTime);

	/// @brief �J�ڏI���������̏���
	void OnTransitionEnd();

	/// @brief �m�[�h�^�C�v����V�����m�[�h���쐬����
	/// @param _nodeType �m�[�h�̎��
	/// @param _nodeName �m�[�h��
	/// @return �쐬�����m�[�h���
	AnimNodeInfo* CreateNodeInfoByType(AnimationNode_Base::NodeType _nodeType, const std::string& _nodeName = "Default");

	void Copy(const AnimationController& _other);
	void CopyNodes(const AnimationController& _other);

	void ImGuiTransition();
	void ImGuiCreateNode();
	void ImGuiTransArrow(AnimNodeInfo& _nodeInfo);
	void ImGuiCreateAnimParameter();

	// �m�[�h�����Z�[�u����
	nlohmann::json SaveNodeInfo(AnimNodeInfo& _nodeInfo);
	// �m�[�h�������[�h����
	void LoadNodeInfo(const nlohmann::json& _nodeInfoData);
protected:
	void ImGuiSetting() override;

	/// @brief �A�j���[�V�����I��������
	virtual void OnAnimationFinish();

	/// @brief �A�j���[�V������ύX����
	void OnChangeAnimComplete();
};

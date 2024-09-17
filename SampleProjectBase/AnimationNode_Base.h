#pragma once
#include "AnimationData.h"

#include "AnimTransitionArrow.h"

class BoneList;

/// @brief �A�j���[�V�����m�[�h�Ɋ��N���X
class AnimationNode_Base : public HashiTaku::IImGuiUser
{
public:
	// �m�[�h�̎��
	enum class NodeType
	{
		Single,	// ������̃A�j���[�V����
		Blend,	// �u�����h����Ă���A�j���[�V����
	};

private:
	/// @brief �m�[�h��
	std::string nodeName;

	/// @brief �J�ڂɊւ���f�[�^����������󃊃X�g
	std::list<std::unique_ptr<AnimTransitionArrow>> pFromArrows;

	///// @brief �J�ڂɊւ���f�[�^����������󃊃X�g
	//std::vector<AnimTransitionArrow*> pToArrows;

	/// @brief �m�[�h�̎��
	NodeType nodeType;

	/// @brief �A�j���[�V�����̎���
	float animationTime;

	/// @brief ���[�v�Đ����邩�H
	bool isLoop;

	/// @brief �A�j���[�V�����I�����Ă��邩�H
	bool isFinish;
public:
	AnimationNode_Base(std::string _nodeName, NodeType _type) : nodeName(_nodeName), nodeType(_type), animationTime(0.0f), isLoop(true), isFinish(false) {}
	virtual ~AnimationNode_Base() {}

	// �Đ����̂Ƃ��ɕ\��
	virtual void ImGuiPlaying();

	/// @brief �A�j���[�V�����J�n���ɍs������
	virtual void Begin();

	/// @brief �X�V�������Ăяo��
	virtual void UpdateCall(float _playingRatio, BoneList& _boneList);

	/// @brief �J�ڂ��邩�m�F����
	/// @return �J�ڐ�f�[�^ nullptr�Ȃ�J�ڂ��Ȃ�
	AnimTransitionArrow* CheckTransition();

	/// @brief �A�j���[�V������ǉ�����
	/// @param _animData �A�j���[�V������
	virtual void SetAnimationData(const std::string& _animName) = 0;

	// �m�[�h�����Z�b�g
	void SetNodeName(const std::string& _nodeName);

	// ���[�v���邩�Z�b�g
	void SetIsLoop(bool _isLoop);

	/// @brief �I���t���O���Z�b�g����
	void SetFinish();

	// �A�j���[�V�����J�ږ���ǉ�
	void AddTransitionArrow(std::unique_ptr<AnimTransitionArrow> _setArrow);

	// �m�[�h���擾
	std::string GetNodeName() const;

	// �m�[�h�̎�ނ��擾
	NodeType GetNodeType() const;

	// �Đ����Ԃ��擾
	float GetAnimationTime() const;

	/// @brief ���[�v�Đ��t���O���擾
	/// @return ���[�v�Đ��t���O
	bool GetIsLoop() const;

	// �A�j���[�V�����I���t���O���擾����
	bool GetIsFinish() const;

	/// @brief �A�j���[�V�����̃g�����X�t�H�[���擾
	/// @param _outTransform �i�[����{�[���g�����X�t�H�[��
	/// @param _boneId �{�[����ID
	/// @param _requestRatio �擾����w��̃A�j���[�V��������
	virtual void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const = 0;
protected:
	/// @brief �X�V�������s��
	/// @param �Đ�����
	/// @param _boneList �{�[�����X�g
	virtual void Update(float _playingRatio, BoneList& _boneList) = 0;

	// �Đ����Ԃ��Z�b�g����
	void SetAnimationTime(float _time);

	void ImGuiSetting() override;
};


#pragma once

#include "AnimationData.h"

class BoneList;

// �A�j���[�V�����m�[�h�Ɋ��N���X
class AnimationNode_Base
{
public:
	// �m�[�h�̎��
	enum class NodeType
	{
		Single,	// ������̃A�j���[�V����
		Blend	// �u�����h����Ă���A�j���[�V����
	};

private:
	/// @brief �m�[�h��
	std::string nodeName;

	/// @brief �m�[�h�̎��
	NodeType nodeType;

	/// @brief �A�j���[�V�����̎���
	float animationTime;

	/// @brief ���[�v�Đ����邩�H
	bool isLoop;
public:
	AnimationNode_Base(std::string _nodeName, NodeType _type) : nodeName(_nodeName), nodeType(_type), animationTime(0.0f), isLoop(true) {}
	virtual ~AnimationNode_Base() {}

	// �Đ����̂Ƃ��ɕ\��
	virtual void ImGuiPlaying();

	/// @brief �X�V�������s��
	/// @param �Đ�����
	/// @param _boneList �{�[�����X�g
	virtual void Update(float _playingRatio, BoneList& _boneList) = 0;

	/// @brief �A�j���[�V������ǉ�����
	/// @param _animData �A�j���[�V������
	virtual void SetAnimationData(const std::string& _animName) = 0;

	// �m�[�h�����Z�b�g
	void SetNodeName(const std::string& _nodeName);

	// �m�[�h���擾
	std::string GetNodeName() const;

	// �m�[�h�̎�ނ��擾
	NodeType GetNodeType() const;

	// �Đ����Ԃ��擾
	float GetAnimationTime() const;

	/// @brief ���[�v�Đ��t���O���擾
	/// @return ���[�v�Đ��t���O
	bool GetIsLoop() const;

	/// @brief �A�j���[�V�����̃g�����X�t�H�[���擾
	/// @param _transforms �i�[����{�[���g�����X�t�H�[���z��
	/// @param _boneNum �{�[���̐�
	/// @param _requestKeyNum �擾�������L�[��
	virtual void GetAnimTransform(std::vector<BoneTransform>& _transforms, u_int _boneNum, u_int _requestKeyNum) const = 0;
protected:

	// �Đ����Ԃ��Z�b�g����
	void SetAnimationTime(float _time);
};


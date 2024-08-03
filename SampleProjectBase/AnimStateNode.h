#pragma once

#include "AnimationData.h"

// �A�j���[�V�����J�ڂŎg�p����1�m�[�h�N���X
class AnimStateNode
{
	/// @brief �m�[�h��
	std::string nodeName;

	/// @brief �A�j���[�V�����f�[�^
	AnimationData* pAnimationData;

	/// @brief ���[�v�Đ����邩�H
	bool isLoop;
public:
	AnimStateNode() : nodeName(""), pAnimationData(nullptr), isLoop(true) {}
	AnimStateNode(std::string _nodeName) : nodeName(_nodeName), pAnimationData(nullptr), isLoop(true) {}
	~AnimStateNode() {}

	void ImGuiSetting();

	/// @brief �A�j���[�V����
	/// @param _animData �A�j���[�V�����f�[�^
	void SetAnimationData(AnimationData& _animData);

	// �m�[�h���擾
	std::string GetNodeName() const;

	// �A�j���[�V�����f�[�^���擾����
	const AnimationData& GetAnimationData() const;

	/// @brief ���[�v�Đ��t���O���擾
	/// @return ���[�v�Đ��t���O
	bool GetIsLoop() const;
};
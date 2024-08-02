#pragma once

#include "AnimStateNode.h"

#include "Asset_Base.h"

/// @brief �A�j���[�V�����J�ڂ��Ǘ�����N���X
class AnimationController : public Asset_Base
{
	/// @brief �A�j���[�V�����m�[�h�z��
	std::unordered_map<std::string, std::unique_ptr<AnimStateNode>> pAnimationNodes;

	/// @brief �Đ����̃A�j���[�V����
	AnimStateNode* pCurrentAnimNode;

	// ��
	float deltaChangeTime = 0.5f;

public:
	AnimationController() : pCurrentAnimNode(nullptr) {}
	~AnimationController() {}

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

	/// @brief �A�j���[�V�����������Ă邩�Ԃ�
	/// @param _animName �A�j���[�V�����̖��O
	/// @return �A�j���[�V���������邩�H
	bool IsHaveAnim(const std::string& _animName);
};


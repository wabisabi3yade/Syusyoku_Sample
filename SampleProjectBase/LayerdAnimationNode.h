#pragma once
#include "AnimationNode_Base.h"

/// @brief ���ʂ��Ƃɕ����ăA�j���[�V��������m�[�h
class LayerdAnimationNode : public AnimationNode_Base
{
	// ����㔼�g�A�����g�̍Đ�����������̂�2�����Ή����܂��A�K�v�ɂȂ�����g��

	/// @brief ��b�ƂȂ�A�j���[�V����
	AnimationData* pBaseAnimation;

	/// @brief �u�����h����A�j���[�V����
	AnimationData* pBlendAnimation;

	/// @brief �ǂ�����u�����h�A�j���[�V�������Đ����邩�w�肷��{�[��ID
	int beginBlendBoneId;
public:
	/// @brief �R���X�g���N�^
	/// @param _nodeName �m�[�h��
	LayerdAnimationNode(const std::string& _nodeName);
	~LayerdAnimationNode() {}


};


#pragma once

#include "InterpolateKind.h"

class AnimationNode_Base;

/// @brief �A�j���[�V�����J�ڂ̏�������ݒ肷����
class AnimTransitionArrow : public HashiTaku::IImGuiUser
{
	/// @brief �J�ڂ������
	std::function<bool()> condition;

	/// @brief	�J�ڌ��A�j���m�[�h
	AnimationNode_Base* pFromNode;

	/// @brief	�J�ڐ�A�j���m�[�h
	AnimationNode_Base* pToNode;

	/// @brief �J�ڐ�̃A�j���[�V�����̎w�芄��
	float transTargetRatio;

	/// @brief �J�ڎ���
	float transitionTime;

	/// @brief �J�ڂ������߂�Ƃ��ɗ��p����C�[�W���O
	HashiTaku::EaseKind easeKind;

	/// @brief �A�j���[�V�����ԂŎg�p�����ԕ��@
	HashiTaku::AnimInterpolateKind interpolateKind;
public:
	/// @brief �R���X�g���N�^
	/// @param _pFromNode �J�ڑO
	/// @param _pToNode �J�ڐ�
	/// @param _transTargetRatio �J�ڐ�A�j���[�V��������(0.0�`1.0)
	/// @param _transitionTime �J�ڎ���
	/// @param _condition �J�ڏ���
	/// @param _easeKind �C�[�W���O�̎��
	AnimTransitionArrow(AnimationNode_Base& _pFromNode, AnimationNode_Base& _pToNode, float _transTargetRatio = 0.0f, float _transitionTime = 0.2f, std::function<bool()> _condition = nullptr, HashiTaku::EaseKind _easeKind = HashiTaku::EaseKind::Linear);

	~AnimTransitionArrow() {}

	/// @brief �J�ڏ�����B�����Ă��邩�m�F
	/// @return �B�����Ă��邩�H
	bool CheckTransition();

	/// @brief �������Z�b�g
	/// @param _condition bool�^�߂�l�̏���
	void SetCondition(std::function<bool()> _condition);

	/// �C�[�W���O�̎�ނ��Z�b�g
	void SetEaseKind(HashiTaku::EaseKind _easeKind);

	// �J�ڂ̕�Ԏ�ނ��Z�b�g
	void SeInterpolateKind(HashiTaku::AnimInterpolateKind _interpolateKind);

	// �J�ڐ�̃m�[�h���擾����
	AnimationNode_Base& GetToNode();

	// �J�ڐ�̃A�j���[�V�����������擾
	float GetTargetRatio() const;

	// �J�ڎ��Ԃ��擾
	float GetTransitionTime() const;

	// �C�[�W���O�̎�ނ��擾
	HashiTaku::EaseKind GetEaseKind() const;

	// ��Ԃ̎�ނ��擾
	HashiTaku::AnimInterpolateKind GetInterpolateKind() const;
private:
	void ImGuiSetting() override;
};


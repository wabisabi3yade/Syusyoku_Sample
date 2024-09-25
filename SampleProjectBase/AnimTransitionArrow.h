#pragma once
#include "TransCondition_Base.h"
#include "InterpolateKind.h"
#include "AnimationParameters.h"

class AnimationNode_Base;

/// @brief �A�j���[�V�����J�ڂ̏�������ݒ肷����
class AnimTransitionArrow : public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	/// @brief �J�ڏ������i�[���郊�X�g
	std::list<std::unique_ptr<TransCondition_Base>> conditionList;

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
	/// @param _fromNode �J�ڑO
	/// @param _toNode �J�ڐ�
	AnimTransitionArrow(AnimationNode_Base& _fromNode, AnimationNode_Base& _toNode);
	virtual ~AnimTransitionArrow() {}

	/// @brief �J�ڏ�����B�����Ă��邩�m�F
	/// @return �B�����Ă��邩�H
	bool CheckTransition();

	/// @brief ���ɏ�����ǉ�
	void AddCondition(conditionValType& _val);

	// �J�ڏI�����̑J�ڐ�̃A�j���[�V�����������Z�b�g
	void SetTransTargetRatio(float _transTargetRatio);

	// �J�ڂɂ����鎞�Ԃ��Z�b�g
	void SetTransitonTime(float _transTime);

	// �C�[�W���O�̎�ނ��Z�b�g
	void SetEaseKind(HashiTaku::EaseKind _easeKind);

	// �J�ڂ̕�Ԏ�ނ��Z�b�g
	void SeInterpolateKind(HashiTaku::AnimInterpolateKind _interpolateKind);

	// �J�ڐ�̃m�[�h���擾����
	AnimationNode_Base& GetToNode() const;

	// �J�ڐ�̃A�j���[�V�����������擾
	float GetTargetRatio() const;

	// �J�ڎ��Ԃ��擾
	float GetTransitionTime() const;

	// �C�[�W���O�̎�ނ��擾
	HashiTaku::EaseKind GetEaseKind() const;

	// ��Ԃ̎�ނ��擾
	HashiTaku::AnimInterpolateKind GetInterpolateKind() const;

	/// @brief �Z�[�u����
	/// @return �Z�[�u�f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data) override;
protected:
	void ImGuiSetting() /*override*/;
};


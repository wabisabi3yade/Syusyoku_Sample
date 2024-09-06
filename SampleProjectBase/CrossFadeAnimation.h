#pragma once
#include "IAnimInterpolate.h"

class AnimationNode_Base;
class BoneList;
struct BoneTransform;

/// @brief �A�j���[�V�����Ԃ̕�ԂɎg�p����N���X�t�F�[�h��ԃN���X
class CrossFadeAnimation : public IAnimInterpolate
{
	/// @brief �J�ڌ��A�j���[�V����
	AnimationNode_Base* pFromNode;

	/// @brief �J�ڐ�A�j���[�V����
	AnimationNode_Base* pToNode;

	/// @brief �J�ڂ̃E�F�C�g
	float transitionWeight;

	/// @brief �J�ڎ���
	float transitionTime;

	/// @brief ���݂̑J�ڎ���
	float elapsedTime;

	/// @brief �C�[�W���O���
	HashiTaku::EaseKind easeKind;

	/// @brief �J�ڌ��̊���
	float curFromRatio;

	/// @brief �J�ڐ�̊���
	float curToRatio;
public:
	CrossFadeAnimation();
	~CrossFadeAnimation() {}
	
	/// @brief �J�n����
	/// @param _fromNode �J�ڌ��A�j���[�V�����m�[�h
	/// @param _toNode �J�ڐ�A�j���[�V�����m�[�h
	/// @param  _transitionTime �J�ڎ��� 
	/// @param _fromStartRatio �J�ڊJ�n���̑J�ڌ��̃A�j���[�V��������
	/// @param _toStartRatio �J�ڊJ�n���̑J�ڐ�̃A�j���[�V��������
	/// @param _easeKind �E�G�C�g��ω�������Ƃ��̃C�[�W���O
	void Begin(AnimationNode_Base& _fromNode, AnimationNode_Base& _toNode, float _transitionTime, float _fromStartRatio, float _toStartRatio, HashiTaku::EaseKind _easeKind);

	/// @brief �X�V����
	/// @param _updateBones�@�X�V����{�[�����X�g 
	/// @param _playSpeed �A�j���[�V�����S�̑��x
	void Update(BoneList& _updateBones, float _playSpeed);

	// �J�ڌ��̊������Z�b�g
	void SetFromCurRatio(float _fromRatio);

	// �J�ڐ�̊������Z�b�g
	void SetToCurRatio(float _toRatio);

	/// @brief �J�ڏI���������擾
	/// @return �J�ڏI���������H
	bool GetIsTransitionEnd() const;

	// �J�ڌo�ߎ��Ԃ��擾����
	float GetElapsedTime() const override;

	// �J�ڎ��Ԃ��擾 
	float GetTransitionTime() const override;
private:

	/// @brief �J�ڎ��Ԃ�i�߂�
	/// @param _deltaTime �Đ����x���l�������o�ߎ���
	void ProgressTime(float _deltaTime);

	/// @brief �g�����W�V�����ɂ���ԏ���
	/// @param _fromTransforms �J�ڌ��̃g�����X�t�H�[��
	/// @param _toTransforms �J�ڐ�̃g�����X�t�H�[��
	/// @param _outTransforms ��Ԃ����g�����X�t�H�[��
	void Interpolate(const BoneTransform& _fromTransforms, const BoneTransform& _toTransforms, BoneTransform& _outTransforms);
};


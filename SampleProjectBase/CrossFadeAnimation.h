#pragma once
#include "IAnimInterpolate.h"

class AnimNodePlayer_Base;
class BoneList;
struct BoneTransform;

/// @brief �A�j���[�V�����Ԃ̕�ԂɎg�p����N���X�t�F�[�h��ԃN���X
class CrossFadeAnimation : public IAnimInterpolate
{
	/// @brief �J�ڌ��A�j���[�V����
	AnimNodePlayer_Base* pFromNodePlayer;

	/// @brief �J�ڐ�A�j���[�V����
	AnimNodePlayer_Base* pToNodePlayer;

	BoneList* pAssetBoneList;

	/// @brief �J�ڂ̃E�F�C�g
	float transitionWeight;

	/// @brief �J�ڎ���
	float transitionTime;

	/// @brief ���݂̑J�ڎ���
	float elapsedTime;

	/// @brief �C�[�W���O���
	HashiTaku::EaseKind easeKind;
public:
	CrossFadeAnimation();
	~CrossFadeAnimation() {}

	/// @brief �J�n����
	/// @param _fromNode �J�ڌ��A�j���[�V�����m�[�h
	/// @param _toNode �J�ڐ�A�j���[�V�����m�[�h
	/// @param _updateBones�@�X�V����{�[�����X�g 
	/// @param  _transitionTime �J�ڎ��� 
	/// @param _fromStartRatio �J�ڊJ�n���̑J�ڌ��̃A�j���[�V��������
	/// @param _toStartRatio �J�ڊJ�n���̑J�ڐ�̃A�j���[�V��������
	/// @param _easeKind �E�G�C�g��ω�������Ƃ��̃C�[�W���O
	void Begin(AnimNodePlayer_Base& _fromNode, AnimNodePlayer_Base& _toNode, BoneList& _updateBones, float _transitionTime, HashiTaku::EaseKind _easeKind);

	/// @brief �X�V����
	/// @param _playSpeed �A�j���[�V�����S�̑��x
	void Update(float _playSpeed);

	/// @brief �J�ڎ��Ԃ�i�߂�
	/// @param _deltaTime �Đ����x���l�������o�ߎ���
	void ProgressTime(float _deltaTime);

	/// @brief �J�ڏI���������擾
	/// @return �J�ڏI���������H
	bool GetIsTransitionEnd() const;

	// �J�ڌo�ߎ��Ԃ��擾����
	float GetElapsedTime() const override;

	// �J�ڎ��Ԃ��擾 
	float GetTransitionTime() const override;

	/// @brief �J�ڃE�F�C�g���擾
	/// @return �J�ڃE�F�C�g
	float GetTransitionWeight() const;
private:
	/// @brief �g�����W�V�����ɂ���ԏ���
	/// @param _fromTransforms �J�ڌ��̃g�����X�t�H�[��
	/// @param _toTransforms �J�ڐ�̃g�����X�t�H�[��
	/// @param _outTransforms ��Ԃ����g�����X�t�H�[��
	void Interpolate(const BoneTransform& _fromTransforms, const BoneTransform& _toTransforms, BoneTransform& _outTransforms);
};


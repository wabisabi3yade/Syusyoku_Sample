#pragma once

#include "AnimationData.h"

// �{�[���̃g�����X�t�H�[��
struct BoneTransform
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 scale;
	DirectX::SimpleMath::Quaternion rotation;
};

/// @brief �t���[�����Ƃ̃{�[���̃L���b�V��
struct BoneCache
{
	std::vector<BoneTransform> transform;	// �{�[���̃g�����X�t�H�[��
	float deltaTime_s;	// �O�t���[������̎���
	bool isEnable;	// �g�p���Ă��邩
};

// �A�j���[�V�����J�ڂŎg�p����1�m�[�h�N���X
class AnimStateNode
{
	/// @brief �A�j���[�V�����f�[�^
	AnimationData* pAnimationData;

	/// @brief 1�t���[���O�̃{�[���̃g�����X�t�H�[��
	BoneCache lastBoneCache;

	/// @brief 2�t���[���O�̃{�[���̃g�����X�t�H�[��
	BoneCache secondLastBoneCache;
public:
	AnimStateNode() : pAnimationData(nullptr) {}
	~AnimStateNode() {}

	/// @brief �A�j���[�V����
	/// @param _animData �A�j���[�V�����f�[�^
	void SetAnimationData(AnimationData& _animData);

	/// @brief �t���[�����Ƃ̃{�[���̃L���b�V�����X�V����
	/// @param boneTransforms �{�[���̃g�����X�t�H�[��
	/// @param _deltaTime �O��t���[������̎���(s)
	void UpdateCache(std::vector<BoneTransform>& boneTransforms, float _deltaTime);

	// �A�j���[�V�����f�[�^���擾����
	const AnimationData& GetAnimationData();

	/// @brief �A�j���[�V�����J�ڂŕ�Ԃ����{�[���̃g�����X�t�H�[����Ԃ�
	/// @param _nextAnimation ���̃A�j���[�V�����m�[�h
	/// @param _blendTime �J�ڂ��鎞��
	/// @return �{�[���̃g�����X�t�H�[��
	std::vector<BoneTransform> CalcTranslation(AnimStateNode& _nextAnimation, float _blendTime);

	/// @brief ���̃A�j���[�V�����܂ł�������ԂőJ�ڂ���
	/// @param _nextAnimation ���̃A�j���[�V����
	/// @param _blendTime �J�ڂ��鎞��
	/// @return �{�[���̃g�����X�t�H�[��
	std::vector<BoneTransform> InertInterpolation(AnimStateNode& _nextAnimation, float _blendTime);

	BoneTransform CalcInertTransform();
};


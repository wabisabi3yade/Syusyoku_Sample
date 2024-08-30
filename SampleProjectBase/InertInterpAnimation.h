#pragma once
#include "Bone.h"

class AnimationData;
class SingleAnimationNode;

/// @brief  ������Ԃōs��
class InertInterpAnimation
{
	/// @brief �t���[�����Ƃ̃{�[���̃L���b�V��
	struct BoneCache
	{
		std::vector<BoneTransform> transform;	// �{�[���̃g�����X�t�H�[��
		float deltaTime_s{0.0f};	// �O�t���[������̎���
		bool isEnable{ false };	// �g�p���Ă��邩
	};

	/// @brief �J�ڏ��
	struct TransitionBase
	{
		float x0;   // ��������
		float v0;   // �������x
		float t1;   // �J�ڏI���̎���
		float a0;   // ���������x
		float A;
		float B;
		float C;
	};

	struct VectorTransition : public TransitionBase
	{
		DirectX::SimpleMath::Vector3 normalizeVector;
	};

	struct QuatTransition : public TransitionBase
	{
		DirectX::SimpleMath::Vector3 axis;  // ���x�N�g��
	};

	/// @brief ���W�̑J�ڏ��
	std::vector<VectorTransition> positionTransition;

	/// @brief �X�P�[���̑J�ڏ��
	std::vector<VectorTransition> scaleTransition;

	/// @brief ��]�̑J�ڏ��
	std::vector<QuatTransition> rotationTransition;

	/// @brief 1�t���[���O�̃{�[���̃g�����X�t�H�[��
	BoneCache lastBoneCache;

	/// @brief 2�t���[���O�̃{�[���̃g�����X�t�H�[��
	BoneCache secondLastBoneCache;

	/// @brief �J�ڂ����Ƃ��̃g�����X�t�H�[��
	std::vector<BoneTransform> changeTimeTransform;

public:
	InertInterpAnimation() {}
	~InertInterpAnimation() {}

	/// @brief �J�ڎ��̌v�Z������
	/// @param _nextAnimation �J�ڐ�̃A�j���[�V�����̎p��
	/// @param _blendTime �u�����h����
	/// @return �v�Z�����������H
	bool Calculate(const std::vector<BoneTransform>& _nextAnimation, float _blendTime);

	/// @brief �{�[���̃L���b�V�����X�V����
	/// @param boneTransforms �X�V����{�[���̃g�����X�t�H�[��
	void UpdateBoneCache(std::vector<BoneTransform>& boneTransforms);

	// �e�p�����[�^�̃u�����h�l���v�Z����
	DirectX::SimpleMath::Vector3 CalcBlendPos(u_int _boneIdx, float _blendingTime);
	DirectX::SimpleMath::Vector3 CalcBlendScale(u_int _boneIdx, float _blendingTime);
	DirectX::SimpleMath::Quaternion CalcBlendRot(u_int _boneIdx, float _blendingTime);
private:

	/// @brief ���W�̏����v�Z
	/// @param _boneIdx �{�[����ID
	/// @param _blendTime �J�ڎ���
	void BoneInitTransition(u_int _boneIdx, const std::vector<BoneTransform>& _animData, float _blendTime);

	/// @brief Vector3�^�̊�����Ԍv�Z
	/// @param _transition �J�ڏ��
	/// @param _requestPose �J�ڐ�̃p�����[�^
	/// @param _lastPose 1�t���[���O�̃p�����[�^
	/// @param _secondLastPose 2�t���[���O�̃p�����[�^
	/// @param _secondToLastDeltaTime 2�t���[���O����1�t���[���O�܂ł�deltaTime
	/// @param _blendTime �J�ڎ���
	void InitTransition(VectorTransition& _transition, const DirectX::SimpleMath::Vector3& _requestPose, const DirectX::SimpleMath::Vector3& _lastPose, const DirectX::SimpleMath::Vector3& _secondLastPose, float _blendTime);

	/// @brief Quaternion�^�̊�����Ԍv�Z
	/// @param _transition �J�ڏ��
	/// @param _requestPose �J�ڐ�̃p�����[�^
	/// @param _lastPose 1�t���[���O�̃p�����[�^
	/// @param _secondLastPose 2�t���[���O�̃p�����[�^
	/// @param _blendTime �J�ڎ���
	void InitTransition(QuatTransition& _transition, const DirectX::SimpleMath::Quaternion& _requestPose, const DirectX::SimpleMath::Quaternion& _lastPose, const DirectX::SimpleMath::Quaternion& _secondLastPose, float _blendTime);

	/// @brief x0��v0�����܂�����ԂŁA�c��̒l���v�Z����
	/// @param _transition �J�ڏ��
	/// @param _blendTime �J�ڎ���
	void CalcTransition(TransitionBase& _transition, float _blendTime);

	/// @brief �u�����h�l���v�Z����i�X�J���[�j
	/// @param _transition �J�ڏ��
	/// @param _blendingTime �u�����h�̌��ݎ���
	/// @return �u�����h�l
	float CalcBlend(const TransitionBase& _transition, float _blendingTime);
};
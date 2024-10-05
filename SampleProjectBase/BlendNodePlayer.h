#pragma once
#include "AnimNodePlayer_Base.h"
#include "BlendAnimationNode.h"

class AnimationParameters;

/// @brief �u�����h�m�[�h���Đ�
class AnimBlendNodePlayer : public AnimNodePlayer_Base
{
	/// @brief �Đ����ɕK�v�ɂȂ鎲���Ƃ̃p�����[�^
	struct AxisPlayParameter
	{
		/// @brief �u�����h�l�̖ڕW���l�|�C���^
		const float* pTargetBlendValue{ nullptr };

		/// @brief 1�t���[���O�̃u�����h�l�̖ڕW�l
		float lastTargetBlendValue{ 0.0f };

		/// @brief ���݂̃u�����h�l
		float curBlendValue{ 0.0f };

		/// @brief �ύX�����u�Ԃ̃u�����h�l
		float changeBlendValue { 0.0f };

		/// @brief ���݂̃u�����h�ړ�����
		float curBlendMoveTime{ 0.0f };
	};

	/// @brief �u�����h�őΉ����Ă��鎲��
	static constexpr int MAX_AXIS_CNT = 2;

	std::vector<AxisPlayParameter> axisPlayParameters;

	/// @brief �A�j���[�V�����p�����[�^���X�g
	const AnimationParameters* pAnimParameters;

	/// @brief �Đ����̃u�����h�m�[�h
	const BlendAnimationNode* pBlendNode;
public:
	AnimBlendNodePlayer(const AnimationParameters& _animParameters, const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform);
	~AnimBlendNodePlayer() {}

private:
	/// @brief ����������
	void Init();

	/// @brief �u�����h�A�j���[�V�����̍X�V����
	void Update() override;

	/// @brief �X�V�ł��邩�m�F
	/// @return �X�V�ł��邩�H
	bool IsCanUpdate();

	/// @brief �u�����h�l���ړ�����
	void MoveCurrentBlend();

	/// @brief �A�j���[�V�����X�V����
	void AnimationUpdate();

	/// @brief 1�݂̂̃A�j���[�V�����u�����h����
	/// @param _blendingData �u�����h�f�[�^
	void SingleAnimationUpdate(const std::vector<BlendAnimationNode::BlendingData>& _blendingData);

	/// @brief 1�݂̂̃A�j���[�V�����u�����h����
	/// @param _blendingData �u�����h�f�[�^
	void TwoAnimationUpdate(const std::vector<BlendAnimationNode::BlendingData>& _blendingData);
	
	/// @brief 1�݂̂̃A�j���[�V�����u�����h����
	/// @param _blendingData �u�����h�f�[�^
	void SquareAnimationUpdate(const std::vector<BlendAnimationNode::BlendingData>& _blendingData);

	/// @brief ���݂̎������擾
	/// @return ����
	u_int GetBlendAxisCnt() const;

	/// @brief ���[�g���[�V�������擾����i�����ŕK�v�Ȍv�Z���s���j
	/// @param ���݂̍Đ�����
	/// @return ���݂̊����̃��[�g���[�V�������W
	DirectX::SimpleMath::Vector3 GetRootMotionPos(float _ratio) override;

	/// @brief ���[�g���[�V�������擾����i�����ŕK�v�Ȍv�Z���s���j
	/// @param ���݂̍Đ�����
	/// @return ���݂̊����̃��[�g���[�V������]��
	DirectX::SimpleMath::Quaternion GetRootMotionRot(float _ratio) override;
};


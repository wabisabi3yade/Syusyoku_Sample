#pragma once
#include "AnimNodePlayer_Base.h"

class SingleAnimationNode;

/// @brief �P��A�j���[�V�����̃m�[�h�Đ�����
class AnimSingleNodePlayer : public AnimNodePlayer_Base
{
public:
	AnimSingleNodePlayer(const AnimationNode_Base& _singleNode, BoneList& _boneList, Transform& _transform);
	~AnimSingleNodePlayer() {}

private:
	void Update(std::vector<BoneTransform>& _outTransforms) override;

	/// @brief ���[�g���[�V�����̍��W�ړ����x���v�Z����
	/// @param _controllerSpeed �R���g���[���[���x
	void CalcRootMotionPosSpeed(float _controllerSpeed) override;

	/// @brief ���[�g���[�V�������擾����i�����ŕK�v�Ȍv�Z���s���j
	/// @param ���݂̍Đ�����
	/// @param _isLoadScaling ���[�h���̃X�P�[���𔽉f���邩
	/// @return ���݂̊����̃��[�g���[�V�������W
	DirectX::SimpleMath::Vector3 GetRootMotionPos(float _ratio, bool _isLoadScaling = true) const override;

	/// @brief ���[�g���[�V�������擾����i�����ŕK�v�Ȍv�Z���s���j
	/// @param ���݂̍Đ�����
	/// @return ���݂̊����̃��[�g���[�V������]��
	DirectX::SimpleMath::Quaternion GetRootMotionRot(float _ratio, bool _isLoadScaling = true) const override;

protected:
	/// @brief ���f���̑傫���������̑傫������ǂꂾ���̔{���Ȃ̂��v�Z����
	/// @return ��������̃��f���X�P�[���{��
	float GetModelScale() const;
};


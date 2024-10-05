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
	void Update() override;

	/// @brief ���[�g���[�V�������擾����i�����ŕK�v�Ȍv�Z���s���j
	/// @param ���݂̍Đ�����
	/// @return ���݂̊����̃��[�g���[�V�������W
	DirectX::SimpleMath::Vector3 GetRootMotionPos(float _ratio) override;

	/// @brief ���[�g���[�V�������擾����i�����ŕK�v�Ȍv�Z���s���j
	/// @param ���݂̍Đ�����
	/// @return ���݂̊����̃��[�g���[�V������]��
	DirectX::SimpleMath::Quaternion GetRootMotionRot(float _ratio) override;

protected:
	/// @brief ���f���̑傫���������̑傫������ǂꂾ���̔{���Ȃ̂��v�Z����
	/// @return ��������̃��f���X�P�[���{��
	float GetModelScale() const;
};


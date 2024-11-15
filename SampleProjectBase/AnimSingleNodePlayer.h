#pragma once
#include "AnimNodePlayer_Base.h"

class SingleAnimationNode;

/// @brief �P��A�j���[�V�����̃m�[�h�Đ�����
class AnimSingleNodePlayer : public AnimNodePlayer_Base
{
	/// @brief �Đ��̃L�[��
	u_int playingFrame;

public:
	/// @brief �R���X�g���N�^
	/// @param _singleNode �P��̃A�j���[�V�����m�[�h
	/// @param _boneList �{�[�����X�g
	/// @param _transform �g�����X�t�H�[��
	AnimSingleNodePlayer(const AnimationNode_Base& _singleNode, BoneList& _boneList, Transform& _transform);
	~AnimSingleNodePlayer() {}

	/// @brief �Đ��L�[�����Z�b�g
	/// @param _playKey �Đ��L�[��
	void SetPlayingKey(u_int _playKey);

	/// @brief �Đ��L�[�����擾����
	/// @return �Đ��L�[
	u_int GetPlayKey() const;

	/// @brief �S�̂̃L�[��
	/// @return �L�[��
	u_int GetAllFrame() const;
private:
	void Update(std::vector<BoneTransform>& _outTransforms) override;

	/// @brief ���[�g���[�V�����̍��W�ړ����x���v�Z����
	void CalcRootMotionPosSpeed() override;

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

	void ImGuiDebug() override;

private:
	
};


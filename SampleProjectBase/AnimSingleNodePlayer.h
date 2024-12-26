#pragma once
#include "AnimNodePlayer_Base.h"

namespace HashiTaku
{
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
		/// @param _isWorldScaling ���[���h���ɑΉ�����X�P�[�����O���邩�H
		/// @return ���݂̊����̃��[�g���[�V�������W
		DXSimp::Vector3 GetRootMotionPos(float _ratio, bool _isWorldScaling = true) const override;

	protected:
		/// @brief ���f���̑傫���������̑傫������ǂꂾ���̔{���Ȃ̂��v�Z����
		/// @return ��������̃��f���X�P�[���{��
		float GetModelScale() const;

		void ImGuiDebug() override;

	private:

	};
}
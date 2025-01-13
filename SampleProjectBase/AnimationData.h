#pragma once
#include "AssetPath_Base.h"

#include "AnimationChannel.h"

namespace HashiTaku
{
	struct BoneTransform;

	/// @brief �A�j���[�V�����̃f�[�^�N���X
	class AnimationData : public AssetPath_Base
	{
	private:
		/// @brief �A�j���[�V�����`�����l�����X�g
		std::vector<std::unique_ptr<AnimationChannel>> pAnimChannels;

		/// @brief ���[�g���[�V�����̃A�j���[�V�����`�����l��
		std::unique_ptr<AnimationChannel> pRootMotionChannels;

		/// @brief �Ή��{�[�����X�g��
		std::string boneListName;

		/// @brief ���[�g���[�V�����ňړ�����ړ����x
		DXSimp::Vector3 rootMovePosPerSec;

		/// @brief ���[�g�{�[���̃C���f�b�N�X(-1�͐ݒ�Ȃ�)
		u_int rootBoneId;

		/// @brief �A�j���[�V�����̎���(s)
		float animationTime_s;

		/// @brief 1�L�[���Ƃ̎���(s)
		float timePerKey_s;

		/// @brief �S�̂̃t���[����
		u_int allFrameCnt;

		/// @brief �E��n���H
		bool isRightHand;
	public:
		AnimationData();
		~AnimationData() {}

		/// @brief �A�j���[�V�����m�[�h��ǉ�����
		/// @param _pAnimNode �A�j���[�V�����m�[�h
		void AddAnimationChannel(std::unique_ptr<AnimationChannel> _pAnimNode);

		/// @brief �{�[��ID����`�����l�����擾
		/// @param _boneIdx �{�[��ID
		/// @return �A�j���[�V�����`�����l��
		const AnimationChannel* FindChannel(u_int _boneIdx) const;

		/// @brief ���[�g���[�V�����֌W�̃p�����[�^�����߂�
		void CalcRootMotion(u_int _rootBoneId);

		// �{�[�����X�g�����Z�b�g
		void SetBoneListName(const std::string& _boneListName);

		// �A�j���[�V�������Ԃ��Z�b�g
		void SetAnimationTime(float _animTime);

		/// @brief �S�̂̃A�j���[�V�����̃t���[�������Z�b�g
		/// @param _allFrameCnt �S�̂̃t���[����
		void SetAllFrameCnt(u_int _allFrameCnt);

		// 1�L�[���Ƃ̂̎��Ԃ��Z�b�g
		void SetTimePerKey(float _timePerKey);

		// �E��n���Z�b�g
		void SetIsRightHand(bool _isRightHand);

		/// @brief �Z�Ԗڂ̃m�[�h�ɑΉ�����{�[�������擾
		/// @param _nodeId �m�[�h��Id
		/// @return �{�[���̖��O
		std::string GetBoneName(u_int _nodeId) const;

		/// @brief �Z�Ԗڂ̃m�[�h�ɑΉ�����{�[��ID���擾
		/// @param _nodeId �m�[�h��Id
		/// @return �{�[��ID
		u_int GetBoneIdx(u_int _nodeId) const;

		// �`�����l���̐����擾
		u_int GetChannelCount() const;

		/// @brief �A�j���[�V�����̃L�[�����擾
		/// @return �A�j���[�V�����̃L�[��
		u_int GetAllAnimationFrame() const;

		/// @brief ��������L�[�ɕϊ�
		/// @param _ratio ����
		/// @return �L�[
		u_int GetRatioToFrame(float _ratio);

		/// @brief �X�P�[�������߂�
		/// @param _boneId �{�[��ID
		/// @param_playingRatio �Đ�����
		/// @param _outScale �A�j���[�V�����̃X�P�[��
		/// @return �Ή������A�j���[�V���������݂��Ă������H
		bool GetScaleByRatio(u_int _boneId, 
			float _playingRatio,
			DXSimp::Vector3& _outScale) const;

		/// @brief �N�H�[�^�j�I�������߂�
		/// @param _boneId �{�[��ID
		/// @param _playingRatio �Đ�����
		/// @param _outRot�A�j���[�V�����̃N�H�[�^�j�I��
		/// @return �Ή������A�j���[�V���������݂��Ă������H
		bool GetQuaternionByRatio(u_int _boneId, 
			float _playingRatio, 
			DXSimp::Quaternion& _outRot) const;

		/// @brief ���W�����߂�
		/// @param _boneId �{�[��ID
		/// @param _playingRatio �Đ�����
		/// @param _outPos �A�j���[�V�����̍��W
		/// @return �Ή������A�j���[�V���������݂��Ă������H
		bool GetPositionByRatio(u_int _boneId,
			float _playingRatio, 
			DXSimp::Vector3& _outPos) const;

		/// @brief �g�����X�t�H�[���擾
		/// @param _boneId �{�[��ID
		/// @param _playingRatio �Đ�����
		/// @param _outTransform ���ʃ{�[���g�����X�t�H�[��
		/// @return �Ή������A�j���[�V���������݂��Ă������H
		bool GetTransformByRatio(u_int _boneId, float _playingRatio, BoneTransform& _outTransform) const;

		/// @brief �X�P�[�������߂�
		/// @param _boneId �{�[��ID
		/// @param _requestKey �擾����L�[
		/// @return �A�j���[�V�����̃X�P�[��
		DXSimp::Vector3 GetScaleByKey(u_int _boneId, u_int _playingKey) const;

		/// @brief �N�H�[�^�j�I�������߂�
		/// @param _boneId �{�[��ID
		/// @param _requestKey �擾����L�[
		/// @return �A�j���[�V�����̃N�H�[�^�j�I��
		DXSimp::Quaternion GetQuaternioneByKey(u_int _boneId, u_int _playingKey) const;

		/// @brief ���W�����߂�
		/// @param _boneId �{�[��ID
		/// @param _requestKey �擾����L�[
		/// @return �A�j���[�V�����̍��W
		DXSimp::Vector3 GetPositioneByKey(u_int _boneId, u_int _playingKey) const;

		/// @brief �g�����X�t�H�[���擾
		/// @param _boneId �{�[��ID
		/// @param _requestKey �擾����L�[
		/// @return �{�[���̃g�����X�t�H�[��
		BoneTransform GetTransformByKey(u_int _boneId, u_int _playingKey) const;

		/// @brief ���[�g���[�V�����̈ړ����x�����߂�(�b
		/// @return ���[�g���[�V�����̈ړ����x
		const DXSimp::Vector3& GetRootMotionPosSpeedPerSec() const;

		/// @brief ���[�g���[�V�����̈ړ����W����������擾����
		/// @param _ratio �擾����������
		/// @return ���[�g���[�V�����ł̈ړ����W
		DXSimp::Vector3 GetRootMotionPos(float _ratio) const;

		/// @brief ���[�g���[�V�����̉�]�ʂ���������擾����
		/// @param _ratio �擾����������
		/// @return ���[�g���[�V�����ł̈ړ����W
		DXSimp::Quaternion GetRootMotionRot(float _ratio) const;

		// �A�j���[�V�����S�̂̎��Ԃ��擾
		float GetAnimationTime() const;

		/// @brief �Z�[�u
		/// @return �f�[�^
		json Save() override;
	};
}
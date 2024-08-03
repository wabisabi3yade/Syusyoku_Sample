#pragma once
#include "Asset_Base.h"

#include "AnimationChannel.h"

/// @brief �A�j���[�V�����̃f�[�^�N���X
class AnimationData : public Asset_Base
{ 
private:
	// �A�j���[�V�����m�[�h���X�g
	std::vector<std::unique_ptr<AnimationChannel>> pAnimChannels;

	/// @brief �A�j���[�V�����̎���(s)
	float animationTime_s;

	/// @brief 1�L�[���Ƃ̎���(s)
	float timePerKey_s;

public:
	AnimationData() : animationTime_s(0.0f), timePerKey_s(0.0f) {}
	~AnimationData() {}

	/// @brief �A�j���[�V�����m�[�h��ǉ�����
	/// @param _pAnimNode �A�j���[�V�����m�[�h
	void AddAnimationChannel(std::unique_ptr<AnimationChannel> _pAnimNode);

	/// @brief �{�[��ID����`�����l�����擾
	/// @param _boneIdx �{�[��ID
	/// @return �A�j���[�V�����`�����l��
	const AnimationChannel* FindChannel(u_int _boneIdx) const;

	// �A�j���[�V�������Ԃ��Z�b�g
	void SetAnimationTime(float _animTime);

	// 1�L�[���Ƃ̂̎��Ԃ��Z�b�g
	void SetTimePerKey(float _timePerKey);

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

	/// @brief �X�P�[�������߂�
	/// @param _boneId �{�[��ID
	/// @param _playingTime �Đ�����
	/// @return �A�j���[�V�����̃X�P�[��
	DirectX::SimpleMath::Vector3 GetScale(u_int _boneId, float _playingTime) const;

	/// @brief �N�H�[�^�j�I�������߂�
	/// @param _boneId �{�[��ID
	/// @param _playingTime �Đ�����
	/// @return �A�j���[�V�����̃N�H�[�^�j�I��
	DirectX::SimpleMath::Quaternion GetQuaternion(u_int _boneId, float _playingTime) const;

	/// @brief ���W�����߂�
	/// @param _boneId �{�[��ID
	/// @param _playingTime �Đ�����
	/// @return �A�j���[�V�����̍��W
	DirectX::SimpleMath::Vector3 GetPosition(u_int _boneId, float _playingTime) const;

	// �A�j���[�V�����S�̂̎��Ԃ��擾
	float GetAnimationTime() const;
};


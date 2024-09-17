#pragma once
#include "IClassNameGetter.h"

/// @brief �A�j���[�V�����̒ʒm�C���^�[�t�F�[�X
class AnimationNotify_Base : public HashiTaku::IImGuiUser, public IClassNameGetter
{
	/// @brief �������
	bool isActive;

public:
	AnimationNotify_Base();
	~AnimationNotify_Base() {}

	/// @brief �A�j���[�V�����̒ʒm�̍X�V
	/// @param _lastPlayingRatio �A�j���[�V�����̑O��̍Đ�����
	/// @param _curPlayingRatio �A�j���[�V�����̌��݂̍Đ�����
	/// @param _isLoop ���������[�v�������H
	virtual void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) = 0;

	// ������Ԃ��Z�b�g
	void SetIsActive(bool _isActive);

	// ������Ԃ��擾
	bool GetIsActive() const;

	virtual std::string ClassNameToStr() override = 0;
protected:
	void ImGuiSetting() override;
};

namespace HashiTaku
{
	// �R���Z�v�g�Ŏ擾�ł���
	template<typename T>
	concept AnimNotifyConcept = std::is_base_of_v<AnimationNotify_Base, T>;
}


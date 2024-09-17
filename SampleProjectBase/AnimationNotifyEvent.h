#pragma once
#include "AnimationNotify_Base.h"

/// @brief �A�j���[�V�����̒ʒm�C�x���g�̊��N���X
class AnimationNotifyEvent : public AnimationNotify_Base
{
	/// @brief �C�x���g�ʒm������A�j���[�V�����̊���
	float eventRatio;
public:
	AnimationNotifyEvent();
	virtual ~AnimationNotifyEvent() {}

	void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) override;

	// �C�x���g�̊������Z�b�g
	void SetEventRatio(float _ratio);

	virtual std::string ClassNameToStr() override = 0;
protected:
	/// @brief �C�x���g�N��������
	virtual void OnEvent() = 0;
};


#pragma once
#include "AnimationNotify_Base.h"

/// @brief �A�j���[�V�����̒ʒm�C�x���g�̊��N���X
class AnimationNotifyEvent : public AnimationNotify_Base
{
	/// @brief �C�x���g�ʒm������A�j���[�V�����̊���
	float eventRatio;
public:
	AnimationNotifyEvent(NotifyType _notifyType);
	virtual ~AnimationNotifyEvent() {}

	void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) override;

	// �C�x���g�̊������Z�b�g
	void SetEventRatio(float _ratio);

	/// @brief �N���[���֐�
	/// @return ���������ʒm�C�x���g
	virtual std::unique_ptr<AnimationNotify_Base> Clone() = 0;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief �C�x���g�N��������
	virtual void OnEvent() = 0;

	void ImGuiSetting() override;
};


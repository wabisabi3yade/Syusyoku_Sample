#pragma once
#include "AnimationNotify_Base.h"

/// @brief �A�j���[�V�����̒ʒm
class AnimationNotifyState : public AnimationNotify_Base
{
	/// @brief �J�n�C�x���g����
	float startEventRatio;

	/// @brief �I���C�x���g����
	float endEventRatio;
public:
	AnimationNotifyState();
	virtual ~AnimationNotifyState() {}

	void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) override;

	// �J�n���̃C�x���g�������Z�b�g����
	void SetStartRatio(float _startRatio);

	// �I�����̃C�x���g�������Z�b�g����
	void SetEndRatio(float _endRatio);

	virtual std::string GetTypeName() const override = 0;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief �C�x���g�J�n����
	virtual void Begin() = 0;

	/// @brief �C�x���g�Ԃ̍X�V����
	virtual void Tick() = 0;

	/// @brief �C�x���g�I������
	virtual void End() = 0;

	void ImGuiSetting() override;
};


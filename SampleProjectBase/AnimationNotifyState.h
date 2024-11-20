#pragma once
#include "AnimationNotify_Base.h"

/// @brief �A�j���[�V�����̒ʒm
class AnimationNotifyState : public AnimationNotify_Base
{
	/// @brief �J�n�C�x���g����
	float startEventRatio;
	/// @brief �J�n�C�x���g�t���[��
	u_int startEventFrame;

	/// @brief �I���C�x���g����
	float endEventRatio;
	/// @brief �I���C�x���g�t���[��
	u_int endEventFrame;
public:

	/// @brief �R���X�g���N�^
	/// @param _notifyType �ʒm�C�x���g���
	AnimationNotifyState(NotifyType _notifyType);
	virtual ~AnimationNotifyState() {}

	void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) override;

	// �J�n���̃C�x���g�������Z�b�g����
	void SetStartRatio(float _startRatio);

	// �I�����̃C�x���g�������Z�b�g����
	void SetEndRatio(float _endRatio);

	/// @brief �N���[���֐�
	/// @return ���������ʒm�C�x���g
	virtual std::unique_ptr<AnimationNotify_Base> Clone() = 0;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief �C�x���g�J�n����
	virtual void Begin() = 0;

	/// @brief �C�x���g�Ԃ̍X�V����
	virtual void Tick() = 0;

	/// @brief �C�x���g�I������
	virtual void End() = 0;

	void ImGuiDebug() override;
};


#pragma once
#include "IAnimationObserver.h"

class PlayerActionController;

/// @brief �v���C���[�̃A�j���[�V�����I�����ɁA�������N�����N���X
class PlayerAnimObserver : public HashiTaku::IAnimationObserver
{
	/// @brief �A�N�V�����R���g���[���[
	PlayerActionController* pActionController;

public:
	PlayerAnimObserver(PlayerActionController& _pActionController);
	~PlayerAnimObserver() {}

	void OnFinishAnimation(u_int _animState) override;
};


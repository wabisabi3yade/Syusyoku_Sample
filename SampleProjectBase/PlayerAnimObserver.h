#pragma once
#include "IAnimationObserver.h"

class PlayerActionController;

/// @brief プレイヤーのアニメーション終了時に、処理を起こすクラス
class PlayerAnimObserver : public HashiTaku::IAnimationObserver
{
	/// @brief アクションコントローラー
	PlayerActionController* pActionController;

public:
	PlayerAnimObserver(PlayerActionController& _pActionController);
	~PlayerAnimObserver() {}

	void OnFinishAnimation(u_int _animState) override;
};


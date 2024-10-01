#pragma once
#include "AnimNodePlayer_Base.h"

class AnimationController;

/// @brief AnimationControllerを再生するクラス
class AnimControllPlayer
{
	/// @brief 再生するアニメーションコントローラー
	const AnimationController* pAnimController;

	/// @brief ノード再生機能
	std::unique_ptr<AnimNodePlayer_Base> pNodePlayer;
public:
	AnimControllPlayer(const AnimationController& _animController);
	~AnimControllPlayer() {}

	/// @brief 更新処理
	void Update();


};


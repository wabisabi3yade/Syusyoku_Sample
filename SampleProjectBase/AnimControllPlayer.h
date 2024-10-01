#pragma once
#include "AnimNodePlayer_Base.h"

class AnimationController;

/// @brief AnimationController���Đ�����N���X
class AnimControllPlayer
{
	/// @brief �Đ�����A�j���[�V�����R���g���[���[
	const AnimationController* pAnimController;

	/// @brief �m�[�h�Đ��@�\
	std::unique_ptr<AnimNodePlayer_Base> pNodePlayer;
public:
	AnimControllPlayer(const AnimationController& _animController);
	~AnimControllPlayer() {}

	/// @brief �X�V����
	void Update();


};


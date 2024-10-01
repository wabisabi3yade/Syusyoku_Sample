#pragma once
#include "AnimNodePlayer_Base.h"

class SingleAnimationNode;

/// @brief �P��A�j���[�V�����̃m�[�h�Đ�����
class AnimSingleNodePlayer : public AnimNodePlayer_Base
{
public:
	AnimSingleNodePlayer(const AnimationNode_Base& _singleNode, BoneList& _boneList);
	~AnimSingleNodePlayer() {}

private:
	void Update() override;
};


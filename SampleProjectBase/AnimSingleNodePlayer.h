#pragma once
#include "AnimNodePlayer_Base.h"

class SingleAnimationNode;

/// @brief 単一アニメーションのノード再生する
class AnimSingleNodePlayer : public AnimNodePlayer_Base
{
public:
	AnimSingleNodePlayer(const AnimationNode_Base& _singleNode, BoneList& _boneList);
	~AnimSingleNodePlayer() {}

private:
	void Update() override;
};


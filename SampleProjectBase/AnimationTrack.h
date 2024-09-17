#pragma once
#include "AnimationNotifyEvent.h"
#include "AnimationNotifyState.h"

class AnimationTrack
{
	/// @brief 
	std::list<std::unique_ptr<AnimationNotifyEvent>> pEvents;
	std::list<std::unique_ptr<AnimationNotifyState>> pStates;

public:
	AnimationTrack() {}
	~AnimationTrack() {}
};


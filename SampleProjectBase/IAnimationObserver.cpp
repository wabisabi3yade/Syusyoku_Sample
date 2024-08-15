#include "pch.h"
#include "IAnimationObserver.h"

void HashiTaku::AnimationSubject::AddObserver(IAnimationObserver& _addObserver)
{
	auto itr = std::find(pObservers.begin(), pObservers.end(), &_addObserver);

	if (itr != pObservers.end()) return;

	pObservers.push_back(&_addObserver);
}

void HashiTaku::AnimationSubject::RemoveObserver(IAnimationObserver& _addObserver)
{
	pObservers.remove(&_addObserver);
}

void HashiTaku::AnimationSubject::NotifyFinish(u_int _animState)
{

	for (auto& o : pObservers)
	{
		o->OnFinishAnimation(_animState);
	}
}

#include "pch.h"
#include "IAnimationObserver.h"

namespace HashiTaku
{
	void AnimationSubject::AddObserver(IAnimationObserver& _addObserver)
	{
		auto itr = std::find(pObservers.begin(), pObservers.end(), &_addObserver);
		if (itr != pObservers.end()) return;

		pObservers.push_back(&_addObserver);
	}

	void AnimationSubject::RemoveObserver(IAnimationObserver& _addObserver)
	{
		pObservers.remove(&_addObserver);
	}

	void AnimationSubject::NotifyFinish(u_int _animState)
	{

		for (auto& o : pObservers)
		{
			o->OnFinishAnimation(_animState);
		}
	}
}